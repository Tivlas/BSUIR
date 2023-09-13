from decimal import Decimal
from django.conf import settings
from travel.models import Trip


class Cart:

    def __init__(self, request):
        self.session = request.session
        cart = self.session.get(settings.CART_SESSION_ID)
        if not cart:
            cart = self.session[settings.CART_SESSION_ID] = {}
        self.cart = cart

    def save(self):
        self.session[settings.CART_SESSION_ID] = self.cart
        self.session.modified = True

    def add(self, trip, quantity=1, update_quantity=False):
        trip_id = str(trip.id)
        if trip_id not in self.cart:
            self.cart[trip_id] = {'quantity': 0,
                                  'cost': str(trip.cost)}
        if update_quantity:
            self.cart[trip_id]['quantity'] = quantity
        else:
            self.cart[trip_id]['quantity'] += quantity
        self.save()

    def remove(self, trip):
        trip_id = str(trip.id)
        if trip_id in self.cart:
            del self.cart[trip_id]
            self.save()

    def __iter__(self):
        trip_ids = self.cart.keys()
        trips = Trip.objects.filter(id__in=trip_ids)
        for trip in trips:
            self.cart[str(trip.id)]['trip'] = trip

        for item in self.cart.values():
            item['cost'] = Decimal(item['cost'])
            item['total_cost'] = item['cost'] * item['quantity']
            yield item

    def __len__(self):
        return sum(item['quantity'] for item in self.cart.values())

    def get_total_cost(self):
        return sum(Decimal(item['cost']) * item['quantity'] for item in
                   self.cart.values())

    def clear(self):
        del self.session[settings.CART_SESSION_ID]
        self.session.modified = True
