from django.db import models
from travel.models import Trip
from login.models import MyUser


class Order(models.Model):
    client = models.ForeignKey(MyUser, on_delete=models.CASCADE, null=True)
    creation_date = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return f"Order {self.id}, made on {self.creation_date}."

    def get_total_cost(self):
        return sum(item.get_total_cost() for item in self.items.all())


class OrderItem(models.Model):
    order = models.ForeignKey(
        Order, related_name='items', on_delete=models.CASCADE)
    trip = models.ForeignKey(
        Trip, related_name='order_items', on_delete=models.CASCADE)
    cost = models.DecimalField(max_digits=10, decimal_places=2)
    quantity = models.PositiveIntegerField(default=1)

    def __str__(self):
        return f"{self.id}"

    def get_total_cost(self):
        return self.cost * self.quantity

    def save(self, *args, **kwargs):
        if self.trip:
            self.cost = self.trip.cost

        super().save(*args, **kwargs)
