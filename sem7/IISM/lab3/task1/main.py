import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import tkinter as tk
from tkinter import messagebox
from scipy import stats


# Определяем совместную плотность вероятности f(x, y)
def f_xy(x, y):
    return 0.5 * np.cos(x - y)


# Маргинальная плотность
def marginal_density_x(x):
    return 0.5 * (np.sin(x) + np.cos(x))


def marginal_density_y(y):
    return 0.5 * (np.sin(y) + np.cos(y))


# Метод отклонений для генерации двумерной случайной величины
def generate_sample(n_samples=10000):
    samples_x = []
    samples_y = []

    for _ in range(n_samples):
        while True:
            # Генерируем кандидатов x и y из равномерного распределения на [0, pi/2]
            x_candidate = np.random.uniform(0, np.pi / 2)
            y_candidate = np.random.uniform(0, np.pi / 2)

            # Генерируем случайное число u для метода отклонений
            u = np.random.uniform(0, 0.5)  # Максимальное значение плотности f(x, y) = 0.5

            # Проверяем условие метода отклонений
            if u <= f_xy(x_candidate, y_candidate):
                samples_x.append(x_candidate)
                samples_y.append(y_candidate)
                break

    return np.array(samples_x), np.array(samples_y)


# Функция для вычисления теоретических характеристик
def theoretical_characteristics():
    mean_x = np.pi / 4
    mean_y = np.pi / 4
    var_x = var_y = 1 / 12  # Дисперсия равномерного распределения на [0, pi/2]

    # Для расчета ковариации
    covariance = np.var([mean_x, mean_y]) * (1 / 4)  # Предполагаем, что зависимость линейная

    # Стандартные отклонения
    std_x = np.sqrt(var_x)
    std_y = np.sqrt(var_y)

    # Корреляция
    correlation = covariance / (std_x * std_y)

    return (mean_x, mean_y), (var_x, var_y), correlation


# Функция для вычисления эмпирических характеристик
def empirical_characteristics(x_samples, y_samples):
    mean_x = np.mean(x_samples)
    mean_y = np.mean(y_samples)
    var_x = np.var(x_samples, ddof=1)  # Используем ddof=1 для выборочной дисперсии
    var_y = np.var(y_samples, ddof=1)
    correlation = np.corrcoef(x_samples, y_samples)[0, 1]

    return (mean_x, mean_y), (var_x, var_y), correlation


# Функция для проверки статистических гипотез
def hypothesis_testing(empirical_values, theoretical_values):
    # Проверяем соответствие математического ожидания
    mu_x_diff = np.abs(empirical_values[0][0] - theoretical_values[0][0])
    mu_y_diff = np.abs(empirical_values[0][1] - theoretical_values[0][1])

    # Проверяем соответствие дисперсии
    var_x_diff = np.abs(empirical_values[1][0] - theoretical_values[1][0])
    var_y_diff = np.abs(empirical_values[1][1] - theoretical_values[1][1])

    # Проверяем соответствие корреляции
    correlation_diff = np.abs(empirical_values[2] - theoretical_values[2])

    return mu_x_diff < 0.1, mu_y_diff < 0.1, var_x_diff < 0.01, var_y_diff < 0.01, correlation_diff < 0.1


# Функция для проверки независимости
def independence_test(x_samples, y_samples, alpha=0.05):
    # Создаем двумерную гистограмму
    hist, xedges, yedges = np.histogram2d(x_samples, y_samples, bins=30)

    # Выполняем критерий хи-квадрат
    chi2_stat, p_value, _, _ = stats.chi2_contingency(hist)

    # Проверка гипотезы о независимости
    return p_value > alpha, chi2_stat, p_value


# Функция для построения 3D-графика распределения
def plot_3d_distribution(x_samples, y_samples):
    # Создание 3D-графика
    fig = plt.figure(figsize=(12, 8))
    ax = fig.add_subplot(111, projection='3d')

    # Создание двумерной гистограммы
    hist, xedges, yedges = np.histogram2d(x_samples, y_samples, bins=30, density=True)

    # Создание координат для 3D-графика
    xpos, ypos = np.meshgrid(xedges[:-1] + 0.025, yedges[:-1] + 0.025, indexing="ij")
    xpos = xpos.ravel()
    ypos = ypos.ravel()
    zpos = 0

    # Высота столбиков соответствует гистограмме
    dx = dy = 0.05 * np.ones_like(zpos)
    dz = hist.ravel()

    # Построение гистограммы
    ax.bar3d(xpos, ypos, zpos, dx, dy, dz, zsort='average', alpha=0.6, color='cyan')

    # Создание сетки для графика плотности
    x_vals = np.linspace(0, np.pi / 2, 100)
    y_vals = np.linspace(0, np.pi / 2, 100)
    X, Y = np.meshgrid(x_vals, y_vals)
    Z = f_xy(X, Y)

    # Построение графика плотности
    ax.plot_surface(X, Y, Z, alpha=0.8, color='orange', edgecolor='none')

    # Настройка меток
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Плотность')
    ax.set_title('Гистограмма распределения и график плотности')

    # Установим пределы для осей
    ax.set_xlim(0, np.pi / 2)
    ax.set_ylim(0, np.pi / 2)
    ax.set_zlim(0, 0.5)

    plt.show()


# Функция для построения гистограмм и графиков условных плотностей
def plot_conditional_densities(x_samples, y_samples):
    # Фиксируем значение Y = pi/4
    fixed_y = np.pi / 4
    # Условия для фильтрации выборок
    condition_y = (y_samples > fixed_y - 0.05) & (y_samples < fixed_y + 0.05)
    x_conditional_samples = x_samples[condition_y]

    # Фиксируем значение X = pi/4
    fixed_x = np.pi / 4
    # Условия для фильтрации выборок
    condition_x = (x_samples > fixed_x - 0.05) & (x_samples < fixed_x + 0.05)
    y_conditional_samples = y_samples[condition_x]

    # Диапазон значений для построения плотностей
    x_vals = np.linspace(0, np.pi / 2, 500)
    y_vals = np.linspace(0, np.pi / 2, 500)

    # Создание общего графика
    plt.figure(figsize=(12, 6))

    # Гистограмма и условная плотность для X|Y
    plt.subplot(1, 2, 1)
    plt.hist(x_conditional_samples, bins=50, density=True, alpha=0.6, color='blue',
             label=f'Гистограмма X|Y={fixed_y:.2f}')
    plt.plot(x_vals, conditional_density_x_given_y(x_vals, fixed_y), label='Условная плотность', color='red')
    plt.xlabel('X')
    plt.ylabel('Плотность')
    plt.title(f'Условная плотность X|Y={fixed_y:.2f}')
    plt.legend()

    # Гистограмма и условная плотность для Y|X
    plt.subplot(1, 2, 2)
    plt.hist(y_conditional_samples, bins=50, density=True, alpha=0.6, color='green',
             label=f'Гистограмма Y|X={fixed_x:.2f}')
    plt.plot(y_vals, conditional_density_y_given_x(y_vals, fixed_x), label='Условная плотность', color='orange')
    plt.xlabel('Y')
    plt.ylabel('Плотность')
    plt.title(f'Условная плотность Y|X={fixed_x:.2f}')
    plt.legend()

    plt.tight_layout()
    plt.show()


# Условная плотность X|Y
def conditional_density_x_given_y(x, y):
    return f_xy(x, y) / marginal_density_y(y)


# Условная плотность Y|X
def conditional_density_y_given_x(y, x):
    return f_xy(x, y) / marginal_density_x(x)


# Функция для обработки нажатия кнопки "Сгенерировать"
def on_generate():
    try:
        n_samples = int(entry_samples.get())
        x_samples, y_samples = generate_sample(n_samples)

        # Вычисление теоретических и эмпирических характеристик
        theoretical_vals = theoretical_characteristics()
        empirical_vals = empirical_characteristics(x_samples, y_samples)

        # Проверка гипотез
        independence_hypothesis, chi2_stat, p_value = independence_test(x_samples, y_samples)
        mu_x_hypothesis, mu_y_hypothesis, var_x_hypothesis, var_y_hypothesis, corr_hypothesis = hypothesis_testing(
            empirical_vals, theoretical_vals)

        # Создаем сообщение с результатами
        result_message = (
            f"Корреляция: {empirical_vals[2]:.4f}\n"
            f"Теоретические значения:\n"
            f"Математическое ожидание X: {theoretical_vals[0][0]:.4f}, Y: {theoretical_vals[0][1]:.4f}\n"
            f"Дисперсия X: {theoretical_vals[1][0]:.4f}, Y: {theoretical_vals[1][1]:.4f}\n"
            f"Эмпирические значения:\n"
            f"Математическое ожидание X: {empirical_vals[0][0]:.4f}, Y: {empirical_vals[0][1]:.4f}\n"
            f"Дисперсия X: {empirical_vals[1][0]:.4f}, Y: {empirical_vals[1][1]:.4f}\n"
            f"Результаты проверки гипотез:\n"
            f"Гипотеза о математическом ожидании X: {'Не отклоняем' if mu_x_hypothesis else 'Отклоняем'}\n"
            f"Гипотеза о математическом ожидании Y: {'Не отклоняем' if mu_y_hypothesis else 'Отклоняем'}\n"
            f"Гипотеза о дисперсии X: {'Не отклоняем' if var_x_hypothesis else 'Отклоняем'}\n"
            f"Гипотеза о дисперсии Y: {'Не отклоняем' if var_y_hypothesis else 'Отклоняем'}\n"
            f"Гипотеза о корреляции: {'Не отклоняем' if corr_hypothesis else 'Отклоняем'}\n"
            f"Статистика хи-квадрат: {chi2_stat:.4f}\n"
            f"P-значение: {p_value:.4f}\n"
        )

        messagebox.showinfo("Результаты", result_message)

        # Построение 3D-графика
        plot_3d_distribution(x_samples, y_samples)

        # Построение гистограмм и графиков условных плотностей
        plot_conditional_densities(x_samples, y_samples)

    except ValueError as e:
        messagebox.showerror("Ошибка", str(e))


# Создаем главное окно
root = tk.Tk()
root.title("Генерация двумерной НСВ")

# Создаем интерфейс
label_samples = tk.Label(root, text="Введите количество выборок:")
label_samples.pack()

entry_samples = tk.Entry(root)
entry_samples.pack()

button_generate = tk.Button(root, text="Сгенерировать", command=on_generate)
button_generate.pack()

# Запускаем главный цикл
root.mainloop()