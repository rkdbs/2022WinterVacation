#include <stdio.h>
#include <mysql/mysql.h> // libmysql:x64-windows
#include "db_config.h" // mysql ��������

int main(void) {
	MYSQL mysql;
	mysql_init(&mysql);
	// mysql ����õ�, ���н� NULL ��ȯ
	MYSQL* connection = mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0);

	if (connection == NULL)
		printf("DB ���� ����\n");
	else
		printf("DB ���� ����\n");

	int query_result;
	// INSERT
	//char insert_q[50] = "INSERT INTO korea VALUES('TOKYO', 10000)";
	// 
	//// Query ����
	//query_result = mysql_query(connection, insert_q);
	//if (query_result == 0)
	//	printf("%s ����\n", insert_q);
	//else
	//	printf("%s ����\n", insert_q);

	// UPDATE
	//char update_q[100] = "UPDATE korea SET population = 10000 WHERE city = 'TOKYO'";
	// 
	//// Query ����
	//query_result = mysql_query(connection, update_q);

	//if (query_result == 0)
	//	printf("%s ����\n", update_q);
	//else
	//	printf("%s ����\n", update_q);

	// DELETE
	//char delete_q[100] = "DELETE FROM korea WHERE city = 'TOKYO'";
	
	//// Query ����
	//query_result = mysql_query(connection, delete_q);

	//if (query_result == 0)
	//	printf("%s ����\n", delete_q);
	//else
	//	printf("%s ����\n", delete_q);

	// SELECT
	char select_q[100] = "SELECT * FROM korea";

	// Query ����
	query_result = mysql_query(connection, select_q);
	if (query_result == 0)
		printf("%s ����\n", select_q);
	else
		printf("%s ����\n", select_q);

	MYSQL_RES* select_result;
	select_result = mysql_store_result(connection);
	MYSQL_ROW sql_row;
	
	while (TRUE) {
		// �� ��(data)�� sql_row�� �޴´�
		sql_row = mysql_fetch_row(select_result);

		// �� �̻� ���� ���� ������
		if (sql_row == NULL)
			break;
		
		// DB�� int�� �����͵� ���ڿ��� �޴´�(sql_row[1])
		printf("%s %s\n", sql_row[0], sql_row[1]);
	}

	mysql_close(&mysql); // mysql ����
}