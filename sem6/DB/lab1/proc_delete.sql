CREATE OR REPLACE PROCEDURE delete_val(p_id IN NUMBER) IS
BEGIN
    DELETE FROM MyTable WHERE id = p_id;
END;