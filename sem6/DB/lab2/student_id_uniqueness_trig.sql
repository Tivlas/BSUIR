CREATE OR REPLACE TRIGGER check_student_id_uniqueness
BEFORE INSERT ON students
FOR EACH ROW
DECLARE
    duplicate_count NUMBER;
BEGIN
    SELECT COUNT(*) INTO duplicate_count
    FROM students
    WHERE student_id = :new.student_id;

    IF duplicate_count > 0 THEN
        RAISE_APPLICATION_ERROR(-20001, 'Student ID must be unique ' || :new.student_id);
    END IF;
END;