package servlet;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import controller.ConnectionControl;
import model.Sensor;
import model.TrashBin;

/**
 * Servlet implementation class servlet
 */
@WebServlet("/servlet")
public class servlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public servlet() {
        super();
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		if(request.getParameter("upload") != null){
			String[] params = request.getParameter("upload").split(":");
			ConnectionControl.getInstance().upload(params[0], params[1], params[2]);
			response.getWriter().append("OK");
		}else if(request.getParameter("subscribe") != null){
			response.getWriter().append(ConnectionControl.getInstance().subscribe()+"");
		}else{			
		    // set the attribute in the request to access it on the JSP
		    request.setAttribute("listData", ConnectionControl.getInstance().getSensors());
		    RequestDispatcher rd = getServletContext().getRequestDispatcher("/Home.jsp");
		    rd.forward(request, response);
		}
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doGet(request, response);
	}
}
