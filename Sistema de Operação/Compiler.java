import org.stringtemplate.v4.*;
import java.util.HashMap;



@SuppressWarnings("CheckReturnValue")
public class Compiler extends TablanBaseVisitor<String> {

   private HashMap<String, String> var = new HashMap<String, String>();
   private STGroup template = new STGroupFile("Tablan.stg");
   private ST st = template.getInstanceOf("typeTable1Declaration");
   private String currentEvent = "";
   private String tableName = "";


   @Override public String visitProgram(TablanParser.ProgramContext ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitStat(TablanParser.StatContext ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitCreateTable1(TablanParser.CreateTable1Context ctx) {
      //String res = null;
      //return visitChildren(ctx);
      //return res;
      STGroup template = new STGroupFile("Tablan.stg");
      ST st = template.getInstanceOf("typeTable1Declaration");
      String s = ctx.ID().getText();
      tableName = s;
      currentEvent = "typeTable";
      //System.out.println(s);
      try{
		String c = ctx.extensionT().getText();
		st.add("tableName", s);
		st.add("column1", "name");
		st.add("column2", "number");
		st.add("type1", "text");
		st.add("type2", "integer");
		st.add("c1Header", "Name:");
		
      }catch(Exception e){
      	//System.out.println(c);
      	this.st.add("tableName", s);
      	visit(ctx.formulaDeclaration(0));
      	visit(ctx.formulaDeclaration(1));

      	

      }
      this.st.render();
      System.out.println(this.st.render());
      currentEvent = "";
      return "";
   }

   @Override public String visitRandomValueForm(TablanParser.RandomValueFormContext ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitRenameTable(TablanParser.RenameTableContext ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitExtensionTable(TablanParser.ExtensionTableContext ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitCreateFor(TablanParser.CreateForContext ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitAtribute1(TablanParser.Atribute1Context ctx) {

	  if(currentEvent == "typeTable"){
	  	   this.st.add("column2", ctx.tp(0).getText());
      	this.st.add("type2", ctx.tp(1).getText());
	  }else{
	    var.put(ctx.tp(0).getText(), ctx.tp(1).getText());
	    //System.out.println(var);
	  }
	  this.st.render();
      return "";
   }

   @Override public String visitAtribute2(TablanParser.Atribute2Context ctx) {
      
      this.st.add("column1", ctx.tp(0).getText());
      //System.out.println(ctx.tp(0).getText());
      this.st.add("type1", ctx.tp(1).getText());
      this.st.add("c1Header", ctx.STR().getText());
      
      return "";
   }

   @Override public String visitAtribute3(TablanParser.Atribute3Context ctx) {
      
      //System.out.println(ctx.tp().getText());
      this.st.add("column2", ctx.tp().getText());
      this.st.add("type2", ctx.ID(0).getText());
      
      return "";
   }

   @Override public String visitAtribute4(TablanParser.Atribute4Context ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitAtribute5(TablanParser.Atribute5Context ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitAtribute6(TablanParser.Atribute6Context ctx) {
      STGroup template = new STGroupFile("Tablan.stg");
      ST stt = template.getInstanceOf("createTable");
      String s = ctx.STR().getText();
      stt.add("tableName", tableName);

      stt.render();
      	  
	   return "";	

      
     
   }

   @Override public String visitAtribute7(TablanParser.Atribute7Context ctx) {
      STGroup template = new STGroupFile("Tablan.stg");
      ST stt = template.getInstanceOf("createTable");
      stt.add("t" , ctx.tp(0).getText());
      //System.out.println(ctx.tp(0).getText());

      
 	  return "";
      
   }

   @Override public String visitWhencase(TablanParser.WhencaseContext ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitRemoveform(TablanParser.RemoveformContext ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitWhenform(TablanParser.WhenformContext ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitAddTable1(TablanParser.AddTable1Context ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitAddTable2(TablanParser.AddTable2Context ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitGetIdType1(TablanParser.GetIdType1Context ctx) {
      //String res = null;
      //return visitChildren(ctx);
      //return res;
      //ctx.ID().getText();
      return "";
   }

   @Override public String visitGetType2(TablanParser.GetType2Context ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitGetType7(TablanParser.GetType7Context ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitPrint1(TablanParser.Print1Context ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitReadin(TablanParser.ReadinContext ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitRead4(TablanParser.Read4Context ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitPrint4(TablanParser.Print4Context ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitAddToNextPrevLine(TablanParser.AddToNextPrevLineContext ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitIsString(TablanParser.IsStringContext ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitExpr5(TablanParser.Expr5Context ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitLength(TablanParser.LengthContext ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitGetId(TablanParser.GetIdContext ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitRead2(TablanParser.Read2Context ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitExpr2(TablanParser.Expr2Context ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitGetId2(TablanParser.GetId2Context ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitAtribute8(TablanParser.Atribute8Context ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }

   @Override public String visitSignals(TablanParser.SignalsContext ctx) {
      String res = null;
      return visitChildren(ctx);
      //return res;
   }
}
