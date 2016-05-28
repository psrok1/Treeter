package com.treeter_client;

import com.treeter_client.Model.GroupModel;
import com.treeter_client.Model.GroupTreeModel;

import javax.swing.*;
import java.awt.*;

public class MainView
{
    private JFrame frame;
    private GroupTreeModel groupTreeModel;
    private GroupTreePanel groupTreePanel;

    private GroupMessageView messageView;
    private GroupMemberView memberView;
    private GroupSubgroupView subgroupView;

    private GroupWaitingPanel waitingPanel;

    private JPanel cardPanel;

    public MainView(GroupTreeModel model)
    {
        this.groupTreeModel = model;

        frame = new JFrame();
        frame.setSize(800, 600);
        frame.setLayout(new BorderLayout());
        frame.getContentPane().setBackground(new Color(0xA6, 0x80, 0xB8));
        // @TODO line below to remove
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        UIManager.put("Tree.collapsedIcon", new ImageIcon(getClass().getResource("/tree-closed.png")));
        UIManager.put("Tree.expandedIcon", new ImageIcon(getClass().getResource("/tree-opened.png")));

        groupTreePanel = new GroupTreePanel(groupTreeModel.getRoot());
//        groupTreePanel.setGroupSelectListener(new GroupTreeSelectListener() {
//            @Override
//            public void groupSelected(GroupTreeModelGroup group) {
//                System.out.println(group.absolutePath);
//                groupTreeModel.setActiveGroup(group.absolutePath);
//            }
//        });

        messageView = new GroupMessageView();
        memberView = new GroupMemberView();
        subgroupView = new GroupSubgroupView();

        JTabbedPane tabbedPane = new JTabbedPane();
        JPanel panel3 = new JPanel();
        tabbedPane.addTab("Konwersacja", messageView);
        tabbedPane.addTab("Uczestnicy", memberView);
        tabbedPane.addTab("Podgrupy", subgroupView);

        waitingPanel = new GroupWaitingPanel();

        cardPanel = new JPanel(new CardLayout());
        cardPanel.add(waitingPanel, "WaitingLayout");
        cardPanel.add(tabbedPane, "GroupLayout");

        // @TODO TESTOWO
        CardLayout cardLayout = (CardLayout) cardPanel.getLayout();
        cardLayout.show(cardPanel, "WaitingLayout");

        frame.add(groupTreePanel, BorderLayout.WEST);
        frame.add(cardPanel, BorderLayout.CENTER);
    }

    public void updateGroup(GroupModel group)
    {
        messageView.updateGroup(group);
        memberView.updateGroup(group);
        subgroupView.updateGroup(group);

        CardLayout cardLayout = (CardLayout) cardPanel.getLayout();
        cardLayout.show(cardPanel, "GroupLayout");
    }

    public void lockWithWaitingMessage(String message)
    {
        CardLayout cardLayout = (CardLayout) cardPanel.getLayout();
        cardLayout.show(cardPanel, "WaitingLayout");
    }

    public void attachController(MainController controller)
    {

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
