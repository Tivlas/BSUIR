CREATE TABLE students (
    student_id NUMBER NOT NULL,
    student_name VARCHAR2(20) NOT NULL,
    gr_id NUMBER NOT NULL,
    CONSTRAINT student_id_pk PRIMARY KEY (student_id),
    CONSTRAINT group_id_fk FOREIGN KEY (gr_id) REFERENCES groups (id)
)