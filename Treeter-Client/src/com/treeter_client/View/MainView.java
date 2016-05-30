/**
 * TODO:
 * Rozwikłać sprawę, czy jestem pendingApproval
 * Moja grupa została usunięta: jak aktualnie mam ją aktywną, powinna zniknąć z widoku
 */

package com.treeter_client.View;

import com.treeter_client.MainController;
import com.treeter_client.Message.ErrorCodeResponse;
import com.treeter_client.Model.GroupModel;
import com.treeter_client.Model.GroupTreeModel;

import javax.swing.*;
import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

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

        JPanel emptyPanel = new JPanel();
        emptyPanel.setBackground(new Color(0xA6, 0x80, 0xB8));

        cardPanel = new JPanel(new CardLayout());
        cardPanel.add(waitingPanel, "WaitingLayout");
        cardPanel.add(tabbedPane, "GroupLayout");
        cardPanel.add(emptyPanel, "EmptyLayout");

        // @TODO TESTOWO
        CardLayout cardLayout = (CardLayout) cardPanel.getLayout();
        cardLayout.show(cardPanel, "EmptyLayout");

        frame.add(groupTreePanel, BorderLayout.WEST);
        frame.add(cardPanel, BorderLayout.CENTER);
    }

    public void setModel(GroupTreeModel model)
    {
        this.groupTreeModel = model;
        this.groupTreePanel.setRoot(model.getRoot());
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
        waitingPanel.setText(message);
        CardLayout cardLayout = (CardLayout) cardPanel.getLayout();
        cardLayout.show(cardPanel, "WaitingLayout");
    }

    public void lockWithEmptyLayout()
    {
        CardLayout cardLayout = (CardLayout) cardPanel.getLayout();
        cardLayout.show(cardPanel, "EmptyLayout");
    }

    public void attachController(MainController controller)
    {
        frame.addWindowListener(new WindowAdapter()
        {
            @Override
            public void windowClosing(WindowEvent e)
            {
                controller.disconnect();
            }
        });

        groupTreePanel.attachController(controller);
        messageView.attachController(controller);
        memberView.attachController(controller);
        subgroupView.attachController(controller);
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

    public void show()
    {
        frame.setVisible(true);
    }

    public void hide()
    {
        frame.setVisible(false);
    }

    public void showError(ErrorCodeResponse errorCode)
    {
        JOptionPane.showMessageDialog(this.frame, errorCode.toString());
    }
}
