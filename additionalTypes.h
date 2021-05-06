/** @file */
#ifndef ADDITIONALTYPES_H
#define ADDITIONALTYPES_H
#include <QString>

/**
    Struktura przechowywującą dane na temat profilu, na potrzeby transportu.
*/
struct profileDataCapsule{
    QString name;
    QString pin;
    QString accnum;
    int profileType; ///< typ profilu (większe/mniejsze uprawnienia)
};
/**
    Struktura przechowywującą dane na temat profilu (z mniejszą ilością danych), na potrzeby transportu.
*/
struct profileDataCapsuleLite{
    QString name;
    QString accnum;
    int profileType; ///< typ profilu (większe/mniejsze uprawnienia)
};




#endif // ADDITIONALTYPES_H
