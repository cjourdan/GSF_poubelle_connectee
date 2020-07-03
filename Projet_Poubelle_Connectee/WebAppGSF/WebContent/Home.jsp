<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<link rel="stylesheet" type="text/css" href="css/style.css">

<title>Home</title>
</head>
<body>


<table class=table-fill>
	<tr>
		<th>ID</th>
		<th data-th="Driver" details><span>Distance minimale</span></th>
		<th>Distance Maximale</th>
	</tr>
<%@page import="java.util.ArrayList" %>
<%@page import="model.*" %>
<%
    ArrayList data = (ArrayList)request.getAttribute("listData");
    for(int i =0;i<data.size();i++){
					try{
						TrashBin t = (TrashBin) data.get(i);
						int id = t.getID();
						out.println("<tr>");
						out.println("<td class=\"text-left\">"+id+"</td>");
						out.println("<td class=\"text-left\">"+t.getActualDistance()+"</td>");
						out.println("<td class=\"text-left\">"+t.getMaxDistance()+"</td>");
						out.println("</tr>");
					}catch(Exception e){
					}
				}
%>
</table>

</body>
</html>