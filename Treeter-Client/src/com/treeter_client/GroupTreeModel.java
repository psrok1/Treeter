package com.treeter_client;

import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import java.util.ArrayList;
import java.util.Date;
import java.util.Enumeration;

/**
 * Created by psrok1 on 27.05.2016.
 */
public class GroupTreeModel
{
    public class GroupMessage
    {
        String author;
        String content;
        Date timestamp;
    }

    public enum GroupState
    {
        Default,
        Active,
        Notified
    }

    public class Group
    {
        String name;
        String absolutePath;
        GroupState state;

        ArrayList<GroupMessage> messages;

        Group(String path)
        {
            this.absolutePath = path;
            this.name = path.substring(path.lastIndexOf('/')+1);
            this.state = GroupState.Default;
        }
    }

    DefaultMutableTreeNode root;
    Group activeGroup;

    public GroupTreeModel()
    {
        Group rootGroup = new Group("/");
        rootGroup.state = GroupState.Active;
        activeGroup = rootGroup;
        root = new DefaultMutableTreeNode(rootGroup);
    }

    private Group getGroupByPath(String path, DefaultMutableTreeNode node, boolean createIfNotExist)
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
            return (Group)node.getUserObject();

        Enumeration<DefaultMutableTreeNode> children = node.children();
        while(children.hasMoreElements())
        {
            DefaultMutableTreeNode child = children.nextElement();
            Group childGroup = (Group) child.getUserObject();
            if(childGroup.name.equals(name))
                return getGroupByPath(child_path, child, createIfNotExist);
        }

        if(createIfNotExist)
        {
            Group group = (Group)node.getUserObject();
            String newGroupPath;
            if(group.absolutePath.equals("/"))
                newGroupPath = "/" + name;
            else
                newGroupPath = group.absolutePath + "/" + name;
            Group newGroup = new Group(newGroupPath);
            DefaultMutableTreeNode newNode = new DefaultMutableTreeNode(newGroup);
            node.add(newNode);
            return getGroupByPath(child_path, newNode, createIfNotExist);
        } else
            return null;
    }

    private Group getGroupByPath(String path, boolean createIfNotExist)
    {
        return getGroupByPath(path.substring(1), root, createIfNotExist);
    }

    public Group addGroup(String path)
    {
        return getGroupByPath(path, true);
    }

    public void deleteGroup(String path)
    {

    }

    public void notifyGroup(String path)
    {
        Group group = getGroupByPath(path, false);

        if(group == null)
            return;

        if(group.state == GroupState.Default)
            group.state = GroupState.Notified;
    }

    public Group setActiveGroup(String path)
    {
        Group group = getGroupByPath(path, false);

        if(group == null)
            return null;

        activeGroup.state = GroupState.Default;
        activeGroup = group;
        activeGroup.state = GroupState.Active;
        return group;
    }

    public void addMessageToGroup(String path, GroupMessage msg)
    {
        Group group = getGroupByPath(path, false);

        if(group == null)
            return;

        group.messages.add(msg);
    }

    public DefaultMutableTreeNode getRoot()
    {
        return root;
    }

    public static void main(String[] args)
    {

    }
}
