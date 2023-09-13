from django import forms
from .models import Trip

class TripForm(forms.ModelForm):
    class Meta:
        model = Trip
        fields = ['name','country','duration','chosen_hotel','departure_date','cost','image']