SELECT *
FROM groups;

SELECT *
FROM students_log
ORDER BY action_date;

SELECT *
FROM students;

CALL restore_students_state('09-FEB-24 11.23.11.505000000 PM');