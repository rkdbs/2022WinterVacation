#include <stdio.h>
#include <mysql/mysql.h> // libmysql:x64-windows
#include "db_config.h" // mysql 계정정보

int main(void) {
	MYSQL mysql;
	mysql_init(&mysql);
	// mysql 연결시도, 실패시 NULL 반환
	MYSQL* connection = mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0);

	if (connection == NULL)
		printf("DB 연결 실패\n");
	else
		printf("DB 연결 성공\n");

	int query_result;
	// INSERT
	//char insert_q[50] = "INSERT INTO korea VALUES('TOKYO', 10000)";
	// 
	//// Query 실행
	//query_result = mysql_query(connection, insert_q);
	//if (query_result == 0)
	//	printf("%s 성공\n", insert_q);
	//else
	//	printf("%s 실패\n", insert_q);

	// UPDATE
	//char update_q[100] = "UPDATE korea SET population = 10000 WHERE city = 'TOKYO'";
	// 
	//// Query 실행
	//query_result = mysql_query(connection, update_q);

	//if (query_result == 0)
	//	printf("%s 성공\n", update_q);
	//else
	//	printf("%s 실패\n", update_q);

	// DELETE
	//char delete_q[100] = "DELETE FROM korea WHERE city = 'TOKYO'";
	
	//// Query 실행
	//query_result = mysql_query(connection, delete_q);

	//if (query_result == 0)
	//	printf("%s 성공\n", delete_q);
	//else
	//	printf("%s 실패\n", delete_q);

	// SELECT
	char select_q[100] = "SELECT * FROM korea";

	// Query 실행
	query_result = mysql_query(connection, select_q);
	if (query_result == 0)
		printf("%s 성공\n", select_q);
	else
		printf("%s 실패\n", select_q);

	MYSQL_RES* select_result;
	select_result = mysql_store_result(connection);
	MYSQL_ROW sql_row;
	
	while (TRUE) {
		// 각 행(data)을 sql_row가 받는다
		sql_row = mysql_fetch_row(select_result);

		// 더 이상 받을 행이 없으면
		if (sql_row == NULL)
			break;
		
		// DB의 int형 데이터도 문자열로 받는다(sql_row[1])
		printf("%s %s\n", sql_row[0], sql_row[1]);
	}

	mysql_close(&mysql); // mysql 종료
}