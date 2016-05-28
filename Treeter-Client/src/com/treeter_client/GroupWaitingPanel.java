package com.treeter_client;

import javax.swing.*;
import java.awt.*;

public class GroupWaitingPanel extends JPanel
{
    private ImageIcon icon;
    JLabel iconLabel;
    JLabel textLabel;
    JPanel centerPane;

    public GroupWaitingPanel()
    {
        this.setLayout(new GridBagLayout());
        centerPane = new JPanel();
        centerPane.setBackground(new Color(0xA6, 0x80, 0xB8));
        centerPane.setLayout(new BoxLayout(centerPane, BoxLayout.Y_AXIS));
        this.setBackground(new Color(0xA6, 0x80, 0xB8));

        icon = new ImageIcon(getClass().getResource("/refresh.png"));
        iconLabel = new JLabel(icon);
        textLabel = new JLabel("");
        textLabel.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 13));
        textLabel.setForeground(Color.white);
        textLabel.setBorder(BorderFactory.createEmptyBorder(30, 0, 0, 0));

        iconLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
        centerPane.add(iconLabel);

        textLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
        centerPane.add(textLabel);

        this.add(centerPane);
    }

    public void setText(String text)
    {
        textLabel.setText(text);
    }
}
