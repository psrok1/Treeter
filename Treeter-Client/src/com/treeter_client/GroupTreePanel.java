package com.treeter_client;

import javax.swing.*;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeSelectionModel;
import java.awt.*;

interface GroupTreeSelectListener {
    void groupSelected(GroupTreeModel.Group group);
}

public class GroupTreePanel extends JPanel
{
    private JTree groupTree;
    private DefaultTreeModel treeModel;

    public class GroupTreeCellRenderer extends DefaultTreeCellRenderer {

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
            GroupTreeModel.Group group = (GroupTreeModel.Group)(node.getUserObject());
            switch(group.state)
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

    public void update()
    {
        this.treeModel.reload();
    }

    public GroupTreePanel(DefaultMutableTreeNode treeRoot)
    {
        this.setLayout(new BorderLayout());

        treeModel = new DefaultTreeModel(treeRoot);
        groupTree = new JTree(treeModel);
        groupTree.setBackground(new Color(0xA6, 0x80, 0xB8));
        groupTree.putClientProperty("JTree.lineStyle", "None");

        groupTree.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
        DefaultTreeCellRenderer treeRenderer = new GroupTreeCellRenderer();
        treeRenderer.setOpenIcon(null);
        treeRenderer.setClosedIcon(null);
        treeRenderer.setLeafIcon(null);

        groupTree.setCellRenderer(treeRenderer);
        this.add(groupTree, BorderLayout.CENTER);
    }

    public void setGroupSelectListener(GroupTreeSelectListener listener)
    {
        groupTree.addTreeSelectionListener(new TreeSelectionListener() {
            @Override
            public void valueChanged(TreeSelectionEvent e) {
                DefaultMutableTreeNode node = (DefaultMutableTreeNode) groupTree.getLastSelectedPathComponent();

                /* if nothing is selected */
                if (node == null) return;

                listener.groupSelected((GroupTreeModel.Group)node.getUserObject());
                treeModel.nodeStructureChanged(node);
            }
        });
    }
}
