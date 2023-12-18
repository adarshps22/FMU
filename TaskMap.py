from Model import Model, TimeUnit
import copy

class Task:
    def __init__(self, name, raster, timeunit):
        self.name = name
        self.raster = raster
        self.timeunit = timeunit
        self.__models = {}

    def add_model(self, model, priority = 0):
        # self.__models[priority] = copy.deepcopy(model)
        self.__models[priority] = model

    def remove_model(self, model):
        key_to_remove = None
        for key, value in self.__models.items():
            if value == model:
                key_to_remove = key
                break
        # Remove the key-value pair if the key was found
        if key_to_remove is not None:
            del self.__models[key_to_remove]

    def get_models(self):
        return self.__models
    
    def reconfigure_priorities(self):
        models = {k: self.__models[k] for k in sorted(self.__models)}
        if len(models) > 1:
            priority_changed_models = {i : list(self.__models.items())[i - 1][1] for i in range(1, len(models) + 1, 1)}
            self.__models = priority_changed_models
        else:
            self.__models = {0 : list(self.__models.items())[0][1]}
        

    
