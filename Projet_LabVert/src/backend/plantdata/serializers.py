from rest_framework import serializers
from .models import PlantData

class PlantDataSerializer(serializers.ModelSerializer):
    class Meta:
        model = PlantData
        fields = ['temperature', 'humidity', 'created_at']