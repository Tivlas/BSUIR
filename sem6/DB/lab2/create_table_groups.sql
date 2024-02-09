CREATE TABLE groups (
    group_id NUMBER NOT NULL,
    group_name VARCHAR2(20) NOT NULL,
    C_VAL NUMBER NOT NULL,
    CONSTRAINT group_id_pk PRIMARY KEY (group_id)
)