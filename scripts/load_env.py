Import("env")
import os

def load_dotenv(path=".env"):
    values = {}
    if not os.path.exists(path):
        print(f"Warning: {path} not found")
        return values

    with open(path, "r") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#") or "=" not in line:
                continue

            key, _, value = line.partition("=")
            key = key.strip()
            value = value.strip().strip("\"").strip("\'")
            values[key] = value

    return values

env_vars = load_dotenv()
for key, value in env_vars.items():
    env.Append(BUILD_FLAGS=[f'-D{key}=\\"{value}\\"'])
    print(f"Injected {key}")