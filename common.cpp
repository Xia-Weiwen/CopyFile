#include "common.h"

void createSrcFile()
{
    if (!QFile(SrcFile).exists())
    {
        QFile f(SrcFile);
        if (!f.open(QFile::WriteOnly))
        {
            exit(0);
        }
        for (int i = 0; i < 1000; ++i)
        {
            f.write("0000000\r\n");
        }
        f.close();
    }
    if (QFile(CpyFile).exists())
    {
        QFile(CpyFile).remove();
    }
}
