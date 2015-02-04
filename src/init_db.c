#include "init_db.h"
#include "csv2ser.h"
#include "ser2seq.h"
#include "seq2act.h"
#include "db_file.h"

int create_from_data(db_file *dbf, char *csv_filename)
{
    int LEN = 0;

    LEN = csv2ser(csv_filename, dbf);
    printf("Read from CSV: %d\n", LEN);

    LEN = ser2seq(dbf);
    printf("Read from SER: %d\n", LEN);

    seq2act(dbf);
    puts("Finished import");

    return 0;
}
