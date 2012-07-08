#include "emdhelper.h"

EmdHelper::Dictionary EmdHelper::getDictionary(QByteArray blob) {
    int filelength = blob.length();
    int* raw = (int*)(blob.data() + filelength - 16); //Last 4 int's are indice to the subfiles

    EmdHelper::Dictionary dict = { raw[0], raw[1], raw[2], raw[3] };
    return dict;
}
