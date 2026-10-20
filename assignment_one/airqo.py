from typing import TypedDict, List, Set

# define typedict class for a single tuple
class Measurement(TypedDict):
    month: str
    pm2_5: int
    temperature: int
    humidity: int


# initialize measurements list containing objects of type Measurement
measurements: List[Measurement] = []

# populate data
measurements.append(Measurement(month="January", pm2_5=23, temperature=40, humidity=26))
measurements.append(Measurement(month="February", pm2_5=31, temperature=43, humidity=25))
measurements.append(Measurement(month="March", pm2_5=34, temperature=48, humidity=29))
measurements.append(Measurement(month="April", pm2_5=42, temperature=53, humidity=38))
measurements.append(Measurement(month="May", pm2_5=40, temperature=64, humidity=42))
measurements.append(Measurement(month="June", pm2_5=42, temperature=68, humidity=38))
measurements.append(Measurement(month="July", pm2_5=27, temperature=75, humidity=34))
measurements.append(Measurement(month="August", pm2_5=23, temperature=48, humidity=42))
measurements.append(Measurement(month="September", pm2_5=27, temperature=48, humidity=38))
measurements.append(Measurement(month="October", pm2_5=26, temperature=60, humidity=32))
measurements.append(Measurement(month="November", pm2_5=23, temperature=53, humidity=31))
measurements.append(Measurement(month="December", pm2_5=31, temperature=40, humidity=26))


def highest_pm2_5(measurements: List[Measurement]) -> Measurement:
    return max(measurements, key=lambda m: m["pm2_5"])


def average_pm2_5(measurements: List[Measurement]) -> float:
    _sum = sum([mnt["pm2_5"] for mnt in measurements])
    return _sum / len(measurements)


def highest_humidity(measurements: List[Measurement]) -> Measurement:
    return max(measurements, key=lambda m: m["humidity"])


def average_temperature(measurements: List[Measurement]) -> float:
    _sum = sum([mnt["temperature"] for mnt in measurements])
    return _sum / len(measurements)


def lowest_pm2_5(measurements: List[Measurement]) -> Measurement:
    return min(measurements, key=lambda m: m["pm2_5"])


def above_average_pm2_5() -> List[Measurement]:
    return list(filter((lambda m: m["pm2_5"] > average_pm2_5(measurements)), measurements))


def month_pm2_dict():
    months: dict[str, int] = {}

    for mnt in measurements:
        months[mnt["month"]] = mnt["pm2_5"]

    print(f"PM2.5 Dict: {months} \n")
    month = input("Enter a month to retrieve the PM2.5 Concentration: ")

    while month.lower() not in [m.lower() for m in months.keys()]:
        month = input("Sorry, that was wrong. Enter a month to retrieve the PM2.5 Concentration: ")

    print(f"Month: {month.capitalize()} \nConcentration: {months[month.capitalize()]} \n")

    highest = highest_pm2_5(measurements)
    print(f"Highest PM2.5 Concentration: {highest["month"]}")

    average = average_pm2_5(measurements)
    print(f"Average PM2.5 Concentration: {average}")


def airqo_sets():
    pm2_5: Set[int] = set[int]()
    temp: Set[int] = set[int]()
    humidity: Set[int] = set[int]()

    for mnt in measurements:
        pm2_5.add(mnt["pm2_5"])
        temp.add(mnt["temperature"])
        humidity.add(mnt["humidity"])

    print(f"PM2.5 Concentration: {pm2_5}\nTemperature: {temp}\nHumidity: {humidity}")
    print(
        f"PM2.5 Concentration Length: {len(pm2_5)}\nTemperature Length: {len(temp)}\nHumidity Length: {len(humidity)}")

    pm_2_diff = len(measurements) - len(pm2_5)
    hum_diff = len(measurements) - len(humidity)
    temp_diff = len(measurements) - len(temp)

    if max([pm_2_diff, hum_diff, temp_diff]) is pm_2_diff:
        print(
            f"PM2.5 Concentration has the highest number of repeated values with {max([pm_2_diff, hum_diff, temp_diff])} measurements. ")
    elif max([pm_2_diff, hum_diff, temp_diff]) is hum_diff:
        print(
            f"Humidity has the highest number of repeated values with {max([pm_2_diff, hum_diff, temp_diff])} measurements. ")
    else:
        print(
            f"Temperature has the highest number of repeated values with {max([pm_2_diff, hum_diff, temp_diff])} measurements. ")


if __name__ == '__main__':
    # qn 1
    print(f"Qn 1. Month with highest PM2.5 Concentration: {highest_pm2_5(measurements)["month"]}")
    # qn 2
    print(f"Qn 2. Average PM2.5 Concentration: {average_pm2_5(measurements)}")
    # qn 3
    print(f"Qn 3. Month with highest Humidity: {highest_humidity(measurements)["month"]}")
    # qn 4
    print(f"Qn 4. Average Temperature: {average_temperature(measurements)}")
    # qn 5
    print(f"Qn 5. Month with lowest PM2.5 Concentration: {lowest_pm2_5(measurements)["month"]}")
    # qn 6
    print(
        f"Qn 6. Months with PM2.5 Concentration above Average: {[mnt["month"] for mnt in above_average_pm2_5()]}")

    #qn 7
    print("\nQn 7.")
    month_pm2_dict()

    #qn 8

    print("\nQn 8.")
    airqo_sets()