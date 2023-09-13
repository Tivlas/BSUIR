from django.contrib import admin
from .models import Order, OrderItem


class OrderItemInline(admin.TabularInline):
    model = OrderItem
    extra = 0
    raw_id_fields = ['trip']


@admin.register(Order)
class OrderAdmin(admin.ModelAdmin):
    list_display = ['client', 'creation_date']
    inlines = [OrderItemInline]
    list_filter = ['creation_date', 'client']
