package com.treeter_client.Model;

import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeNode;
import java.util.Enumeration;

/**
 * Created by psrok1 on 27.05.2016.
 */
public class GroupTreeModel
{
    DefaultMutableTreeNode root;
    DefaultTreeModel treeModel;
    GroupModel activeGroup;


    public GroupTreeModel()
    {
        GroupModel groupRoot = new GroupModel("/");
        root = new DefaultMutableTreeNode(groupRoot);
        treeModel = new DefaultTreeModel(root);
        activeGroup = null;
    }

    public DefaultTreeModel getUIModel()
    {
        return treeModel;
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
            treeModel.nodeStructureChanged(node);
            return getNodeByPath(child_path, newNode, createIfNotExist);
        } else
            return null;
    }

    private DefaultMutableTreeNode getNodeByPath(String path, boolean createIfNotExist)
    {
        return getNodeByPath(path.substring(1), root, createIfNotExist);
    }

    public GroupModel getGroupByPath(String path, boolean createIfNotExist)
    {
        DefaultMutableTreeNode node = getNodeByPath(path, createIfNotExist);
        return node != null ? (GroupModel)node.getUserObject() : null;
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

        TreeNode parent = node.getParent();
        node.removeFromParent();
        treeModel.nodeStructureChanged(parent);
    }

    public void notifyGroup(GroupModel group)
    {
        if(group.uiState == GroupUIState.Default)
            group.uiState = GroupUIState.Notified;
    }

    public GroupModel setActiveGroup(GroupModel group)
    {
        if(activeGroup != null)
            activeGroup.uiState = GroupUIState.Default;

        activeGroup = group;

        if(group != null)
            activeGroup.uiState = GroupUIState.Active;

        return group;
    }

    public boolean isActiveGroup(GroupModel group)
    {
        return activeGroup == group;
    }

    public DefaultMutableTreeNode getRoot()
    {
        return root;
    }

    public GroupModel getActiveGroup()
    {
        return activeGroup;
    }
}
