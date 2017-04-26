<?php


class UniqueIdList
{
    private $uniqueIds = array();

    public function createNewList($startNumber, $endNumber)
    {
        do {
            $this->uniqueIds = array();
            for ($i = $startNumber; $i <= $endNumber; $i++) {
                $id = md5(uniqid());
                $this->uniqueIds[$i] = $id;
            }
        } while (count($this->uniqueIds) !== count(array_unique($this->uniqueIds)));
    }

    public function printList()
    {
        echo '<h1>Liste der Zugangscodes für die Verkäufer</h1>';
        echo '<div class="code">';

        foreach ($this->uniqueIds as $sellerNumber => $id) {
            echo $id . " = " . $sellerNumber . '<br />';
        }

        echo '</div>';
    }

    public function readFromFile($fileName)
    {
        if (file_exists($fileName)) {
            $lines = file($fileName);

            foreach ($lines as $line) {
                $data = explode(' = ', trim($line));
                $this->uniqueIds[$data[1]] = $data[0];
            }
        }
    }

    public function checkId($sellerNumber, $id)
    {
        return $this->uniqueIds[$sellerNumber] == $id;
    }

    public function getId($sellerNumber)
    {
        return $this->uniqueIds[$sellerNumber];
    }

    public function writeToFile($fileName)
    {
        $fh = fopen($fileName, "w");
        foreach ($this->uniqueIds as $sellerNumber => $id) {
            fwrite($fh, $id . " = " . $sellerNumber . "\n");
        }
        fclose($fh);
    }
}