CREATE OR REPLACE TRIGGER students_log_trigger
AFTER INSERT OR UPDATE OR DELETE ON students
FOR EACH ROW
BEGIN
    IF INSERTING THEN
        INSERT INTO students_log (log_id, action, new_student_id, student_name, gr_id, action_date)
        VALUES (students_log_seq.nextval, 'INSERT', :new.student_id, :new.student_name, :new.gr_id, SYSTIMESTAMP);
    ELSIF UPDATING THEN
        INSERT INTO students_log (log_id, action, old_student_id, new_student_id, student_name, gr_id, action_date)
        VALUES (students_log_seq.nextval, 'UPDATE', :old.student_id, :new.student_id, :new.student_name, :new.gr_id, SYSTIMESTAMP);
    ELSIF DELETING THEN
        INSERT INTO students_log (log_id, action, old_student_id, student_name, gr_id, action_date)
        VALUES (students_log_seq.nextval, 'DELETE', :old.student_id, :old.student_name, :old.gr_id, SYSTIMESTAMP);
    END IF;
END;