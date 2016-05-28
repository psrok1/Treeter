package com.treeter_client.Model;

import javax.swing.tree.DefaultMutableTreeNode;
import java.util.Enumeration;

/**
 * Created by psrok1 on 27.05.2016.
 */
public class GroupTreeModel
{
    DefaultMutableTreeNode root;
    GroupModel activeGroup;

    public GroupTreeModel()
    {
        GroupModel groupRoot = new GroupModel("/");
        root = new DefaultMutableTreeNode(groupRoot);
        activeGroup = null;
    }

    private DefaultMutableTreeNode getNodeByPath(String path, DefaultMutableTreeNode node, boolean createIfNotExist)
    {
        String name;
        String child_path;

        if(path.indexOf('/') == -1) {
            name = path;
            child_path = "";
        }
        else {
            name = path.substring(0, path.indexOf('/'));
            child_path = path.substring(path.indexOf('/')+1);
        }

        if(name.equals(""))
            return node;

        Enumeration<DefaultMutableTreeNode> children = node.children();
        while(children.hasMoreElements())
        {
            DefaultMutableTreeNode child = children.nextElement();
            GroupModel childGroup = (GroupModel) child.getUserObject();
            if(childGroup.name.equals(name))
                return getNodeByPath(child_path, child, createIfNotExist);
        }

        if(createIfNotExist)
        {
            GroupModel group = (GroupModel)node.getUserObject();
            String newGroupPath;
            if(group.absolutePath.equals("/"))
                newGroupPath = "/" + name;
            else
                newGroupPath = group.absolutePath + "/" + name;
            GroupModel newGroup = new GroupModel(newGroupPath);
            DefaultMutableTreeNode newNode = new DefaultMutableTreeNode(newGroup);
            node.add(newNode);
            return getNodeByPath(child_path, newNode, createIfNotExist);
        } else
            return null;
    }

    private DefaultMutableTreeNode getNodeByPath(String path, boolean createIfNotExist)
    {
        return getNodeByPath(path.substring(1), root, createIfNotExist);
    }

    private GroupModel getGroupByPath(String path, boolean createIfNotExist)
    {
        return (GroupModel)getNodeByPath(path, createIfNotExist).getUserObject();
    }

    public GroupModel addGroup(String path)
    {
        return getGroupByPath(path, true);
    }

    public void deleteGroup(String path)
    {
        DefaultMutableTreeNode node = getNodeByPath(path, false);

        if(node == null)
            return;

        node.removeFromParent();
    }

    public void notifyGroup(String path)
    {
        GroupModel group = getGroupByPath(path, false);

        if(group == null)
            return;

        if(group.uiState == GroupUIState.Default)
            group.uiState = GroupUIState.Notified;
    }

    public GroupModel setActiveGroup(String path)
    {
        GroupModel group = getGroupByPath(path, false);

        if(group == null)
            return null;

        if(activeGroup != null)
            activeGroup.uiState = GroupUIState.Default;
        activeGroup = group;
        activeGroup.uiState = GroupUIState.Active;
        return group;
    }

    public DefaultMutableTreeNode getRoot()
    {
        return root;
    }
}
