DECLARE
    v_insert_command VARCHAR2(100);
BEGIN
    v_insert_command := generate_insert_command(1);
    EXCEPTION
        WHEN OTHERS THEN
            DBMS_OUTPUT.PUT_LINE('Error: ' || SQLERRM);
END;