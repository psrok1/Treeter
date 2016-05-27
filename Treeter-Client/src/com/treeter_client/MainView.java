package com.treeter_client;

import javax.swing.*;
import java.awt.*;

public class MainView
{
    private JFrame frame;
    private GroupTreeModel model;

    public MainView(GroupTreeModel model)
    {
        this.model = model;

        frame = new JFrame();
        frame.setSize(800, 600);
        frame.setLayout(new BorderLayout());
        frame.getContentPane().setBackground(new Color(0xA6, 0x80, 0xB8));
        // @TODO line below to remove
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        UIManager.put("Tree.collapsedIcon", new ImageIcon(getClass().getResource("/tree-closed.png")));
        UIManager.put("Tree.expandedIcon", new ImageIcon(getClass().getResource("/tree-opened.png")));

        GroupTreePanel groupTreePanel = new GroupTreePanel(model.getRoot());
        groupTreePanel.setGroupSelectListener(new GroupTreeSelectListener() {
            @Override
            public void groupSelected(GroupTreeModel.Group group) {
                System.out.println(group.absolutePath);
                model.setActiveGroup(group.absolutePath);
            }
        });

        GroupMessageView groupMessageView = new GroupMessageView();
        JTabbedPane tabbedPane = new JTabbedPane();
        JPanel panel2 = new JPanel();
        JPanel panel3 = new JPanel();
        tabbedPane.addTab("Konwersacja", groupMessageView);
        tabbedPane.addTab("Uczestnicy", panel2);
        tabbedPane.addTab("Podgrupy", panel3);

        frame.add(groupTreePanel, BorderLayout.WEST);
        frame.add(tabbedPane, BorderLayout.CENTER);
    }

    public static void main(String[] args)
    {
        GroupTreeModel treeModel = new GroupTreeModel();
        treeModel.addGroup("/TIN");
        treeModel.addGroup("/JPS");
        treeModel.addGroup("/TIN/Projekt");
        treeModel.addGroup("/TIN/Projekt/Kowalski");
        treeModel.addGroup("/TIN/Projekt/Nowak");
        treeModel.addGroup("/TIN/Projekt/Srokosz");
        treeModel.addGroup("/TIN/Projekt/Srokosz/Madrzy ludzie");
        treeModel.addGroup("/TIN/Projekt/Srokosz/Debile");
        treeModel.addGroup("/TIN/Projekt/Srokosz/Treeter");
        MainView view = new MainView(treeModel);
        view.frame.setVisible(true);
    }
}
