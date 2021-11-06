curl -X POST http://localhost:9000/try-me \
    -d "{\"price\": 50.5, \"quantity\": 117, \"symbol\": \"AAPL\"}" \
    -H "Content-Type: text" -H "Solace-delivery-mode: direct"
