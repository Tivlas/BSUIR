CREATE OR REPLACE FUNCTION calculate_total_compensation(p_salary IN NUMBER, p_percent IN INT)
    RETURN NUMBER IS
    v_percent NUMBER;
    v_remuneration NUMBER;
BEGIN
    IF (p_percent < 0 OR p_percent > 100) THEN
        RAISE_APPLICATION_ERROR(-20001, 'Invalid percent value.');
    END IF;
    v_percent := p_percent / 100;
    v_remuneration := (1 + v_percent) * 12 * p_salary;
    RETURN v_remuneration;
EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Error: ' || SQLERRM);
        RETURN NULL;
END;