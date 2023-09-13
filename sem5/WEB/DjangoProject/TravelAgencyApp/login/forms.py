from django import forms
from django.contrib.auth.forms import UserCreationForm
from .models import MyUser
from django.core.validators import RegexValidator
from datetime import date
from django.core.exceptions import ValidationError
from collections import OrderedDict


def age_validator(value):
    today = date.today()
    age = today.year - value.year - \
        int((today.month, today.day) < (value.month, value.day))
    if int(age) < 18:
        raise ValidationError(
            "Only 18+")


class MyUserCreationForm(UserCreationForm):
    email = forms.EmailField(required=True)
    first_name = forms.CharField(max_length=200)
    last_name = forms.CharField(max_length=200)
    date_of_birth = forms.DateField(validators=[age_validator])
    phone_number = forms.CharField(max_length=50,
                                   validators=[RegexValidator(
                                       regex=r'^(\+375 (?:\(29\)|\(33\)|\(44\)|\(25\)) [0-9]{3}-[0-9]{2}-[0-9]{2})$',
                                       message='Pattern: +375 (29/25/33/44) XXX-XX-XX',
                                   )])

    class Meta:
        model = MyUser
        fields = {'username', 'email', 'first_name',
                  'last_name', 'date_of_birth',
                  'phone_number', 'password1',
                  'password2'
                  }

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.fields = OrderedDict(
            (field_name, self.fields[field_name])
            for field_name in [
                'username', 'email', 'first_name', 'last_name', 'date_of_birth',
                'phone_number', 'password1', 'password2'
            ]
        )

    def save(self, commit: bool):
        user = super(MyUserCreationForm, self).save(commit=False)
        user.email = self.cleaned_data['email']
        user.first_name = self.cleaned_data['first_name']
        user.last_name = self.cleaned_data['last_name']
        user.date_of_birth = self.cleaned_data['date_of_birth']
        user.phone_number = self.cleaned_data['phone_number']

        if commit:
            user.save(True)

        return user
