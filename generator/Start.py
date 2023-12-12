from Model import Model, TimeUnit
from Scheduler import Scheduler
from Utility import Utility
from TaskMap import Task
from UMLGenerator import WSDFileCreator
from Clock import Clock, ClockType, VariabilityType, TimeBasedClockType

def create_wsd_file(scheduler):
    creator = WSDFileCreator('example.wsd')
    creator.create_wsd_file()
    creator.add_startuml()

    creator.add_models_to_file(Utility.get_models(scheduler.get_tasks()))

    for model in Utility.get_models(scheduler.get_tasks()):
        creator.add_process(model)
        for clock in model.get_all_clocks():
            if clock.isTimeBased():
                creator.add_process(clock)
    
    creator.add_timestamp_to_file(scheduler.get_end_time(), scheduler.get_global_time_scale())
    creator.add_time_scale(Utility.get_models(scheduler.get_tasks())[0].get_name_without_spaces(), scheduler.get_end_time(), scheduler.get_global_time_unit())
    creator.add_enduml()

def main():    
    A = Model('FMU A')
    B = Model('FMU B')
    C = Model('FMU C')
    D = Model('FMU D')
    E = Model('FMU E')

    clock1 = Clock('Clock 1', TimeUnit.MS)
    clock1.configure_constant_clock(400)
    clock2 = Clock('Clock 2', TimeUnit.MS)
    clock2.configure_constant_clock(1000)
    clock3 = Clock('Clock 3', TimeUnit.MS)
    clock3.configure_constant_clock(500)
    clock4 = Clock('Clock 4', TimeUnit.MS)
    clock4.configure_constant_clock(3000)
    clock5 = Clock('Clock 5', TimeUnit.MS, 2000, 2500)
    clock5.configure_tunable_clock(100)

    #B.add_clock(clock1)
    #B.add_clock(clock4)
    D.add_clock(clock2)  
    D.add_clock(clock3)
    D.add_clock(clock5)

    task = Task('Task 1', 3, TimeUnit.S)
    task.add_model(A, 2)
    task.add_model(B, 3)
    task.add_model(C, 1)

    task2 = Task('Task 2', 3, TimeUnit.S)
    task2.add_model(D)

    scheduler = Scheduler()
    
    #scheduler.add_task(task)
    scheduler.add_task(task2)

    scheduler.normalize_system_time()
    scheduler.configure_local_time()
    scheduler.configure_global_time()

    scheduler.set_end_time(4, TimeUnit.S)

    scheduler.calculate_execution_interval()

    create_wsd_file(scheduler)

if __name__ == "__main__":
    main()