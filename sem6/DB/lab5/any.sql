DELETE FROM uni;

DELETE FROM groups;

DELETE FROM students;

DELETE FROM uni_logs;

DELETE FROM groups_logs;

DELETE FROM students_logs;

INSERT INTO uni (
    uni_name,
    creation_date
) VALUES(
    'u1',
    '11-MAR-24 06.41.24.789000000 PM'
);

INSERT INTO uni (
    uni_name,
    creation_date
) VALUES(
    'u2',
    '16-MAR-24 06.41.24.789000000 PM'
);

INSERT INTO uni (
    uni_name,
    creation_date
) VALUES(
    'u3',
    '18-MAR-24 08.45.45.789000000 PM'
);

UPDATE uni
SET
    creation_date = systimestamp
WHERE
    uni_name = 'u1';

DELETE FROM uni
WHERE
    uni_name = 'u3';

SELECT *
FROM uni_logs
ORDER BY change_date;

SELECT *
FROM uni
ORDER BY uni_id;

CALL func_package.roll_back(to_timestamp('22-MAR-24 07.35.38.246000000 PM'));

CALL func_package.roll_back(1200000);

CALL func_package.report(to_timestamp('1/1/1 1:1:1',
    'YYYY/MM/DD HH:MI:SS'))