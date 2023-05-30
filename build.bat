rem goto end
python scripts/buildVersion.py "11.17.0-50U"
python scripts/buildVersion.py "N11.17.0-50U"
goto end
python scripts/buildVersion.py "11.15.0-47E"
python scripts/buildVersion.py "11.15.0-47J"

python scripts/buildVersion.py "N11.15.0-47E"
python scripts/buildVersion.py "N11.15.0-47J"
:end
pause