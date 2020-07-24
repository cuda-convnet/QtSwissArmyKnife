﻿/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKINPUTDATAFACTORY_HH
#define SAKINPUTDATAFACTORY_HH

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include "SAKDebugPageInputController.hh"

class SAKCRCInterface;
/// @brief handle the input data
class SAKInputDataFactory:public QThread
{
    Q_OBJECT
public:
    SAKInputDataFactory(QObject *parent = Q_NULLPTR);
    ~SAKInputDataFactory();

    /**
     * @brief cookData: handle the input data
     * @param rawData: input data
     * @param parameters: input parameters
     */
    void cookData(QString rawData, SAKDebugPageInputController::InputParametersContext parameters);

    /**
     * @brief crcCalculate: calculate crc for input data
     * @param data: input data
     * @param model: crc parameters model
     * @return crc value
     */
    quint32 crcCalculate(QByteArray data, int model);

    /**
     * @brief rawDataToArray: text to byte array
     * @param rawData: input text
     * @param parameters: input parameters
     * @return byte array
     */
    QByteArray rawDataToArray(QString rawData, SAKDebugPageInputController::InputParametersContext parameters);
protected:
    void run() final;
private:
    struct RawDataStruct {
        QString rawData;
        SAKDebugPageInputController::InputParametersContext parameters;
    };

    QList<RawDataStruct> rawDataList;
    QMutex rawDataListMutex;
    QMutex threadMutex;
    QWaitCondition threadCondition;
    SAKCRCInterface *crcInterface;
    SAKDebugPage *debugPage;
private:
    RawDataStruct takeRawData();
    void innnerCookData(QString rawData, SAKDebugPageInputController::InputParametersContext parameters);
signals:
    // the data can be send directly
    void dataCooked(QByteArray);
};

#endif