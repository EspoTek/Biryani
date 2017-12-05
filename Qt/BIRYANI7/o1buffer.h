#ifndef O1BUFFER_H
#define O1BUFFER_H

#include <QObject>
#define NUM_SAMPLES_PER_CHANNEL 16777215 //2^24 - 1.  Highest number that can be stored in the 24 bit address.

class o1buffer : public QObject
{
    Q_OBJECT
public:
    explicit o1buffer(QObject *parent = 0);
    ~o1buffer();
    void add(int value, int address);
    int get(int address);
private:
    int *buffer;
signals:

public slots:
};

#endif // O1BUFFER_H
