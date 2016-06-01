package com.treeter_client.View;

import com.treeter_client.MainController;
import com.treeter_client.Message.ErrorCodeResponse;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class ConnectView
{
    private JFrame frame;
    private ImageIcon logoImage;
    private JLabel logo, logLabel, serverLabel, nickLabel, passwordLabel;
    private JPanel outsidePanel, insidePanel, buttonPanel;
    private JTextField serverField, nickField;
    private JPasswordField passwordField;
    private JButton acceptButton, registerButton;

    public ConnectView()
    {
        frame = new JFrame();
        frame.setSize(600, 360);
        frame.setLayout(new BorderLayout());
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.getContentPane().setBackground(new Color(0xA6, 0x80, 0xB8));

        logoImage = new ImageIcon(getClass().getResource("/logo-simple.png"));
        logo = new JLabel(logoImage);
        frame.add(logo, BorderLayout.CENTER);

        outsidePanel = new JPanel(new BorderLayout());
        outsidePanel.setBorder(new EmptyBorder(0, 32, 16, 32));
        outsidePanel.setBackground(new Color(0xA6, 0x80, 0xB8));
        frame.add(outsidePanel, BorderLayout.SOUTH);

        insidePanel = new JPanel(new GridBagLayout());
        insidePanel.setBackground(Color.WHITE);
        outsidePanel.add(insidePanel, BorderLayout.CENTER);
        final GridBagConstraints c = new GridBagConstraints();
        c.insets = new Insets(4, 16, 8, 4);

        // Etykieta "Logowanie"
        logLabel = new JLabel("Logowanie");
        c.fill = GridBagConstraints.CENTER;
        c.gridx = 0;
        c.gridy = 0;
        c.gridwidth = 2;
        insidePanel.add(logLabel, c);

        // Pole wprowadzania adresu serwera
        serverLabel = new JLabel("Adres serwera");
        c.fill = GridBagConstraints.HORIZONTAL;
        c.weightx = 0.1;
        c.gridx = 0;
        c.gridy = 1;
        c.gridwidth = 1;
        insidePanel.add(serverLabel, c);

        serverField = new JTextField();
        c.weightx = 0.9;
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridx = 1;
        c.gridy = 1;
        insidePanel.add(serverField, c);

        // Pole wprowadzania nazwy użytkownika
        nickLabel = new JLabel("Nazwa użytkownika");
        c.fill = GridBagConstraints.HORIZONTAL;
        c.weightx = 0.1;
        c.gridx = 0;
        c.gridy = 2;
        insidePanel.add(nickLabel, c);

        nickField = new JTextField();
        c.weightx = 0.9;
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridx = 1;
        c.gridy = 2;
        insidePanel.add(nickField, c);

        // Pole wprowadzania hasla
        passwordLabel = new JLabel("Hasło");
        c.fill = GridBagConstraints.HORIZONTAL;
        c.weightx = 0.1;
        c.gridx = 0;
        c.gridy = 3;
        insidePanel.add(passwordLabel, c);

        passwordField = new JPasswordField();
        c.weightx = 0.9;
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridx = 1;
        c.gridy = 3;
        insidePanel.add(passwordField, c);

        // Panel przycisków
        buttonPanel = new JPanel();
        buttonPanel.setLayout(new GridLayout(1, 2, 64, 0));
        buttonPanel.setComponentOrientation(ComponentOrientation.RIGHT_TO_LEFT);
        buttonPanel.setBackground(Color.WHITE);
        c.weightx = 1;
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridx = 0;
        c.gridy = 4;
        c.gridwidth = 2;
        insidePanel.add(buttonPanel, c);

        // Przyciski
        acceptButton = new JButton("Zaloguj");
        registerButton = new JButton("Zarejestruj");
        buttonPanel.add(acceptButton);
        buttonPanel.add(registerButton);
    }

    public void show()
    {
        frame.setVisible(true);
    }

    public void showError(ErrorCodeResponse error)
    {
        JOptionPane.showMessageDialog(this.frame, error.toString());
    }

    public void hide()
    {
        frame.setVisible(false);
    }

    public void attachController(MainController controller)
    {
        acceptButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                controller.connect(serverField.getText(), false);
            }
        });

        registerButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e)
            {
                controller.connect(serverField.getText(), true);
            }
        });
    }

    public String getNick()
    {
        return nickField.getText();
    }

    public String getPassword()
    {
        return new String(passwordField.getPassword());
    }
}
