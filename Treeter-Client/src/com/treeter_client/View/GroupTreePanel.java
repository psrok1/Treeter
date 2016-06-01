package com.treeter_client.View;

import com.treeter_client.MainController;
import com.treeter_client.Model.GroupModel;

import javax.swing.*;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.*;
import java.awt.*;

interface GroupTreeSelectListener {
    void groupSelected(GroupModel group);
}

public class GroupTreePanel extends JPanel
{
    private JTree groupTree;
    private DefaultTreeModel treeModel;

    public GroupTreePanel(DefaultMutableTreeNode treeRoot)
    {
        this.setLayout(new BorderLayout());

        treeModel = new DefaultTreeModel(treeRoot);
        groupTree = new JTree(treeModel);
        groupTree.setBackground(new Color(0xA6, 0x80, 0xB8));
        groupTree.putClientProperty("JTree.lineStyle", "None");
        groupTree.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
        DefaultTreeCellRenderer treeRenderer = new GroupTreeCellRenderer();
        treeRenderer.setOpenIcon(new ImageIcon(getClass().getResource("/tree-opened.png")));
        treeRenderer.setClosedIcon(new ImageIcon(getClass().getResource("/tree-closed.png")));
        treeRenderer.setLeafIcon(null);

        groupTree.setCellRenderer(treeRenderer);
        this.add(groupTree, BorderLayout.CENTER);
    }

    public void setModel(DefaultTreeModel treeModel)
    {
        this.groupTree.setModel(treeModel);
    }

    public void setGroupSelectListener(GroupTreeSelectListener listener)
    {
        groupTree.addTreeSelectionListener(new TreeSelectionListener() {
            @Override
            public void valueChanged(TreeSelectionEvent e) {
                DefaultMutableTreeNode node = (DefaultMutableTreeNode) groupTree.getLastSelectedPathComponent();
                /* if nothing is selected */
                if (node == null) return;

                listener.groupSelected((GroupModel)node.getUserObject());
                treeModel.nodeStructureChanged(node);
            }
        });
    }

    public void attachController(MainController controller)
    {
        setGroupSelectListener(new GroupTreeSelectListener() {
            @Override
            public void groupSelected(GroupModel group) {
                controller.selectGroup(group);
            }
        });
    }
}

class GroupTreeCellRenderer extends DefaultTreeCellRenderer {

    @Override
    public Color getBackgroundNonSelectionColor() {
        return (null);
    }

    @Override
    public Color getBackgroundSelectionColor() {
        return (null);
    }

    @Override
    public Color getBackground() {
        return (null);
    }

    @Override
    public Component getTreeCellRendererComponent(final JTree tree, final Object value, final boolean sel, final boolean expanded, final boolean leaf, final int row, final boolean hasFocus) {
        final Component ret = super.getTreeCellRendererComponent(tree, value, sel, expanded, leaf, row, hasFocus);

        final DefaultMutableTreeNode node = ((DefaultMutableTreeNode) (value));
        GroupModel group = (GroupModel)(node.getUserObject());
        switch(group.uiState)
        {
            case Notified:
                this.setForeground(Color.YELLOW);
                break;
            case Active:
                this.setForeground(Color.WHITE);
                this.setFont(new Font("default", Font.BOLD, 14));
                break;
            default:
                this.setForeground(Color.WHITE);
                this.setFont(new Font("default", Font.PLAIN, 14));
        }
        if(group.absolutePath.equals("/"))
        {
            this.setText("Treeter");
            this.setFont(new Font("default", Font.ITALIC, 18));
        }
        else {
            this.setText(group.name);
        }
        return ret;
    }
}
