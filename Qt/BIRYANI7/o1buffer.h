#ifndef O1BUFFER_H
#define O1BUFFER_H

#include <QObject>
#include <QVector>
#define NUM_SAMPLES_PER_CHANNEL 16777215 //AKA buffer size, in words.
//16777215 = 2^24 - 1.  Highest number that can be stored in the 24 bit address.
#define DEFAULT_CONVERTED_STREAM_SIZE 4096

class o1buffer : public QObject
{
    Q_OBJECT
public:
    explicit o1buffer(QObject *parent = 0);
    ~o1buffer();
    void add(int value, int address);
    int get(int address);
    int mostRecentAddress = 0;
    QVector<double> *getMany_nofilter(int numToGet, int interval_samples, int delay_sample);
private:
    int *buffer;
    QVector<double> convertedStream;
    void updateMostRecentAddress(int newAddress);
signals:

public slots:
};

#endif // O1BUFFER_H
