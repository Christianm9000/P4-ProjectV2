<?php
// Database connection details
$servername = "mysql100.unoeuro.com";
$port = 3306;
$username = "nexus_energy_dk";
$password = "erA46bR5G2t3awzhm9xf";
$database = "nexus_energy_dk_db_p4";

// Check if the request method is POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Read the raw POST data from the request body
    $data = file_get_contents('php://input');
    
    // Log the received data to a file named log1.txt
    file_put_contents('log1.txt', $data, FILE_APPEND);
    
    // Decode the JSON data into a PHP associative array
    $decodedData = json_decode($data, true);

    // Create a new connection to the MySQL database
    $conn = new mysqli($servername, $username, $password, $database, $port);
    
    // Check if the database connection was successful
    if ($conn->connect_error) {
        // If the connection failed, terminate the script and output an error message
        die("Connection failed: " . $conn->connect_error);
    }

    // Initialize the SQL query for inserting data into the iot_data table
    $sql = "INSERT INTO iot_data (";
    $placeholders = "";
    $values = [];

    // Iterate over the decoded JSON data
    foreach ($decodedData as $key => $value) {
        // Check if the current key is 'gws' and its value is an array
        if ($key === 'gws' && is_array($value)) {
            // Iterate over the 'gws' array
            foreach ($value as $gwsData) {
                // Iterate over each element in the 'gwsData' array
                foreach ($gwsData as $nestedKey => $nestedValue) {
                    // Exclude the 'ts' (timestamp) key from insertion
                    if ($nestedKey !== 'ts') {
                        // Append the column name and placeholder to the SQL query
                        $sql .= "`$nestedKey`, ";
                        $placeholders .= "?, ";
                        // Add the value to the values array
                        $values[] = $nestedValue;
                    }
                }
            }
        } else {
            // Append the column name and placeholder for other keys
            $sql .= "`$key`, ";
            $placeholders .= "?, ";
            // Add the value to the values array
            $values[] = $value;
        }
    }

    // Remove trailing commas from the SQL query and complete the VALUES clause
    $sql = rtrim($sql, ", ") . ") VALUES (" . rtrim($placeholders, ", ") . ")";
    
    // Prepare the SQL statement
    $stmt = $conn->prepare($sql);

    // Check if the statement was prepared successfully
    if (!$stmt) {
        // If preparation failed, terminate the script and output an error message
        die("Error preparing statement: " . $conn->error);
    }

    // Create a string with the types of the values ('s' for string) for binding
    $types = str_repeat('s', count($values));
    
    // Bind the parameters to the SQL statement
    $stmt->bind_param($types, ...$values);

    // Execute the SQL statement
    if (!$stmt->execute()) {
        // If execution failed, terminate the script and output an error message
        die("Error executing statement: " . $stmt->error);
    }

    // Close the prepared statement and the database connection
    $stmt->close();
    $conn->close();
}

// Output a message indicating that the data was received
echo "Data received.";
?>
