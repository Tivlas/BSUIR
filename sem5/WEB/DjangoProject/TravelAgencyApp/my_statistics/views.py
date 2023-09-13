from django.shortcuts import render
from django.db.models import Sum, F
from order.models import Order
from login.models import MyUser
import matplotlib.pyplot as plt
from django.conf import settings
import numpy as np
from django.core.exceptions import PermissionDenied
from os import path


def statistics(request):
    if not request.user.is_superuser:
        raise PermissionDenied("Permission denied")

    clients = MyUser.objects.order_by('last_name', 'first_name')
    client_data = []

    for client in clients:
        orders = client.order_set.all()
        total_sales = orders.annotate(
            total_cost=Sum(F('items__quantity') * F('items__cost'))
        ).aggregate(total_sales=Sum('total_cost'))['total_sales']
        client_data.append(
            {'client': client, 'orders': orders, 'total_sales': total_sales})

    context = {'client_data': client_data}

    pt = dict()

    x = []
    y = []

    for order in Order.objects.all():
        pt[str(order.creation_date.year) + '.' +
           str(order.creation_date.month) + '.' + str(order.creation_date.day)] = 0

    for order in Order.objects.all():
        pt[str(order.creation_date.year) + '.' + str(order.creation_date.month) +
           '.' + str(order.creation_date.day)] += 1

    for tmp in pt:
        x.append(tmp)
        y.append(pt[tmp])

    plt.xlabel('date', fontsize=10)
    plt.ylabel('order amount', fontsize=10)
    plt.plot(x, y)
    if request.method == "GET":
        plt.savefig(path.join(settings.MEDIA_ROOT,
                    'orders_per_day.png'), format='png')
        plt.close()

    amount_orders_per_day = list(pt.values())
    mean_orders_per_day = np.mean(amount_orders_per_day)
    median_orders_per_day = np.median(amount_orders_per_day)
    min_orders_per_day = np.min(amount_orders_per_day)
    max_orders_per_day = np.max(amount_orders_per_day)

    context.update({'mean_orders_per_day': mean_orders_per_day,
                    'median_orders_per_day': median_orders_per_day,
                    'min_orders_per_day': min_orders_per_day,
                    'max_orders_per_day': max_orders_per_day,
                    })

    return render(request, 'statistics.html', context)
