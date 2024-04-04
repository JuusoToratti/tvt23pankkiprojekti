#include "rfidreaderdll.h"
#include <QSerialPortInfo>
#include <QDebug>

RFIDReaderdll::RFIDReaderdll(QObject *parent)
    : QObject(parent)
{
    // Etsi RFID-lukija COM5-portista
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if (info.portName() == "COM5") {
            m_serial.setPort(info);
            if (m_serial.open(QIODevice::ReadOnly)) {
                connect(&m_serial, &QSerialPort::readyRead, this, &RFIDReaderdll::readRFID);
                qDebug() << "RFID-lukija löydetty COM5-portista";
            } else {
                qDebug() << "Sarjaportin avaaminen epäonnistui";
            }
            return;
        }
    }
    qDebug() << "RFID-lukijaa ei löydetty COM5-portista";
}

void RFIDReaderdll::readRFID()
{
    QByteArray data = m_serial.readAll();
    QString cardID = QString::fromLatin1(data).trimmed();
    emit cardDetected(cardID);
}
