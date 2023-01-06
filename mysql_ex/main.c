#include <mysql/mysql.h>
#include <stdio.h>
#include "db_config.h" // ���������� ����ִ� ȯ�漳�� ����

#define CHOP(x) x[strlen(x) - 1] = ' '

int main(void)
{
    MYSQL* connection = NULL, conn;
    MYSQL_RES* sql_result;
    MYSQL_ROW   sql_row;
    int       query_stat;

    char tel[12];
    char query[255];

    mysql_init(&conn);

    connection = mysql_real_connect(&conn, DB_HOST,
        DB_USER, DB_PASS,
        DB_NAME, 3306,
        (char*)NULL, 0);

    if (connection == NULL)
    {
        fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
        return 1;
    }

    //// SELECT ��
    //query_stat = mysql_query(connection, "select * from login");
    //if (query_stat != 0)
    //{
    //    fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
    //    return 1;
    //}

    //sql_result = mysql_store_result(connection);

    //printf("ID ��й�ȣ\n");
    //while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
    //{
    //    printf("%s %s \n", sql_row[0], sql_row[1]);
    //}

    //mysql_free_result(sql_result);

    //printf("Insert Value \n");
    //printf("�̸� :");
    //fgets(name, 12, stdin);
    //CHOP(name);

    //printf("��й�ȣ :");
    //fgets(address, 80, stdin);
    //CHOP(address);

    //// SELEC �� end

    // INSERT��
    sprintf(query, "INSERT INTO korea VALUES"
        "('%s', %d)",
        "TOKYO", 10000);

    query_stat = mysql_query(connection, query);
    // ������ �߻����� ��
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 1;
    }
    // ���࿡ �������� ��
    else {
        printf("%s�� ����", query);
    }
    // INSERT��

    mysql_close(connection);
}