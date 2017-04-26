<?php

class Settings
{
    private $fileName;

    public $minSellerNumber;
    public $maxSellerNumber;
    public $minArticleNumber;
    public $maxArticleNumber;
    public $deadline;

    function __construct()
    {
        $this->readFromFile();
    }

    public function readFromFile()
    {
        $this->fileName = "../Data/settings.txt";

        if (file_exists($this->fileName)) {
            $file = fopen($this->fileName, "r");

            $this->minSellerNumber = rtrim(fgets($file));
            $this->maxSellerNumber = rtrim(fgets($file));
            $this->minArticleNumber = rtrim(fgets($file));
            $this->maxArticleNumber = rtrim(fgets($file));
            $this->deadline = rtrim(fgets($file));
            fclose($file);
        }
    }
} 