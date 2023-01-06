#include <stdio.h>
#include <mysql/mysql.h> // libmysql:x64-windows
#include "db_config.h" // mysql ��������

int main(void) {
	MYSQL mysql;
	mysql_init(&mysql);
	// mysql ����õ�, ���н� NULL ��ȯ
	MYSQL* connection = mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0);

	if (connection == NULL)
		printf("DB ���� ����");
	else
		printf("DB ���� ����");

	mysql_close(&mysql); // mysql ����
}