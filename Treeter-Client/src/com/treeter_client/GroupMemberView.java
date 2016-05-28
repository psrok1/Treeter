package com.treeter_client;

import com.treeter_client.Model.GroupMember;
import com.treeter_client.Model.GroupMemberListModel;
import com.treeter_client.Model.GroupModel;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.util.Vector;

/**
 * Created by psrok1 on 27.05.2016.
 */
public class GroupMemberView extends JPanel
{
    JLabel panelTitle;

    JList<GroupMember> memberList;
    JScrollPane memberScrollPane;

    JPopupMenu membersMenu;
    JMenuItem memberStandard;
    JMenuItem memberModerator;
    JMenuItem memberAccept;
    JMenuItem memberReject;

    JPanel inviteMemberPanel;
    JLabel inviteMemberLabel;
    JTextField inviteMemberField;
    JButton inviteMemberButton;

    public GroupMemberView()
    {
        this.setLayout(new BorderLayout());
        this.setBorder(new EmptyBorder(10, 10, 10, 10));
        this.setBackground(Color.WHITE);
        // nazwa grupy
        panelTitle = new JLabel("Lista użytkowników");
        this.add(panelTitle, BorderLayout.NORTH);

        membersMenu = new JPopupMenu();
        membersMenu.add(memberStandard = new JMenuItem("Odbierz prawa moderatora"));
        membersMenu.add(memberModerator = new JMenuItem("Uczyń moderatorem"));
        membersMenu.add(memberAccept = new JMenuItem("Akceptuj"));
        membersMenu.add(memberReject = new JMenuItem("Wyrzuć"));

        memberList = new JList<GroupMember>();
        memberList.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 13));
        memberList.setSelectionMode(ListSelectionModel.SINGLE_INTERVAL_SELECTION);
        memberList.setLayoutOrientation(JList.VERTICAL);
        memberList.setVisibleRowCount(-1);
        memberScrollPane = new JScrollPane();
        memberScrollPane.getViewport().add(memberList);
        this.add(memberScrollPane, BorderLayout.CENTER);

        inviteMemberPanel = new JPanel(new BorderLayout());
        inviteMemberPanel.setBackground(Color.WHITE);
        this.add(inviteMemberPanel, BorderLayout.SOUTH);

        inviteMemberLabel = new JLabel("Dodaj uczestnika:");
        inviteMemberPanel.add(inviteMemberLabel, BorderLayout.NORTH);

        inviteMemberField = new JTextField();
        inviteMemberPanel.add(inviteMemberField, BorderLayout.CENTER);

        inviteMemberButton = new JButton(new ImageIcon(getClass().getResource("/add.png")));
        inviteMemberButton.setPreferredSize(new Dimension(36, 36));
        inviteMemberPanel.add(inviteMemberButton, BorderLayout.EAST);
    }

    public void updateGroup(GroupModel group)
    {
        GroupMemberListModel model = group.getMemberList();
        memberList.setListData(model.getData());
        memberList.revalidate();
        memberList.repaint();
    }

    public void attachController(MainController controller)
    {

    }
}

class GroupMemberCellRenderer extends DefaultListCellRenderer
{
    private JLabel label;
    private Color textSelectionColor = Color.BLACK;
    private Color backgroundSelectionColor = Color.CYAN;
    private Color textNonSelectionColor = Color.BLACK;
    private Color backgroundNonSelectionColor = Color.WHITE;

    GroupMemberCellRenderer()
    {
        label = new JLabel();
        label.setOpaque(true);
    }

    @Override
    public Component getListCellRendererComponent(
            JList list,
            Object value,
            int index,
            boolean selected,
            boolean expanded) {

        GroupMember member = (GroupMember)value;
        String memberText = member.login;
        switch(member.role)
        {
            case Standard:
                label.setIcon(new ImageIcon(getClass().getResource("/user-standard.png")));
                break;
            case Moderator:
                memberText += " [moderator]";
                label.setIcon(new ImageIcon(getClass().getResource("/user-moderator.png")));
                break;
            case PendingApproval:
                memberText += " [oczekuje na dodanie]";
                label.setIcon(new ImageIcon(getClass().getResource("/user-pending.png")));
                break;
        }
        label.setText(memberText);
        label.setToolTipText("");

        if (selected) {
            label.setBackground(backgroundSelectionColor);
            label.setForeground(textSelectionColor);
        } else {
            label.setBackground(backgroundNonSelectionColor);
            label.setForeground(textNonSelectionColor);
        }

        return label;
    }
}