CREATE TABLE students_log (
    log_id NUMBER NOT NULL,
    action VARCHAR2(6) NOT NULL,
    new_student_id NUMBER,
    old_student_id NUMBER,
    student_name VARCHAR2(20) NOT NULL,
    gr_id NUMBER NOT NULL,
    action_date TIMESTAMP NOT NULL,
    CONSTRAINT log_id_pk PRIMARY KEY (log_id)
);