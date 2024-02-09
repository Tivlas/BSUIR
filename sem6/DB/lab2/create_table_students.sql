CREATE TABLE students (
    student_id NUMBER NOT NULL,
    student_name VARCHAR2(20) NOT NULL,
    group_id NUMBER NOT NULL,
    CONSTRAINT student_id_pk PRIMARY KEY (student_id),
    CONSTRAINT group_id_fk FOREIGN KEY (group_id) REFERENCES groups (group_id)
)