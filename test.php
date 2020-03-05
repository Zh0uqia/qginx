<html>
<body>

<h1>My first PHP page</h1>

<?php
switch($_SERVER['REQUEST_METHOD'])
{
case 'GET': var_dump($_GET); break;
case 'POST': var_dump($_POST); break;
default: break;
}
?>

</body>
</html>
