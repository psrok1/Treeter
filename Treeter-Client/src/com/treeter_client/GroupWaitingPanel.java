package com.treeter_client;

import javax.swing.*;
import java.awt.*;

public class GroupWaitingPanel extends JPanel
{
    private ImageIcon icon;
    JLabel iconLabel;

    public GroupWaitingPanel()
    {
        icon = new ImageIcon(getClass().getResource("/refresh.png"));
        iconLabel = new JLabel(icon);
        this.add(iconLabel, BorderLayout.SOUTH);
    }
}
