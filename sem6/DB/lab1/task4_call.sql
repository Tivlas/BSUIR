DECLARE
    v_insert_command VARCHAR2(4000);
BEGIN
    v_insert_command := generate_insert_command(2);
END;