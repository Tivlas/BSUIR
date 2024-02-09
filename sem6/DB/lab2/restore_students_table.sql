CREATE OR REPLACE PROCEDURE restore_students_state(p_timestamp TIMESTAMP) IS
     cur_time TIMESTAMP;
BEGIN
    cur_time := SYSTIMESTAMP;
    DELETE FROM students;

    FOR lg IN (
        SELECT *
        FROM students_log
        WHERE action_date <= p_timestamp
        ORDER BY action_date
    ) LOOP
        IF lg.action = 'INSERT' THEN
            INSERT INTO students (student_id, student_name, gr_id)
            VALUES (lg.new_student_id, lg.student_name, lg.gr_id);
        ELSIF lg.action = 'UPDATE' THEN
            UPDATE students
            SET student_name = lg.student_name, gr_id = lg.gr_id, student_id = lg.new_student_id
            WHERE student_id = lg.old_student_id;
        ELSIF lg.action = 'DELETE' THEN
            DELETE FROM students
            WHERE student_id = lg.old_student_id;
        END IF;
    END LOOP;

    DELETE FROM students_log
    WHERE action_date >= cur_time;
END;