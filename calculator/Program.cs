// See https://aka.ms/new-console-template for more information

// Используется алгоритм Shunting-yard (https://en.wikipedia.org/wiki/Shunting-yard_algorithm)

// Считываем строку с выражением
// Делаем класс парсера, который будет считывать одну ноду за раз
// Ноды по алгоритму Shunting-yard преобразуем в постфиксную нотацию (так намного легче вычислять выражение)
// Передаем полученный стэк в класс, который будет исполнять выражение

// Итого нужны классы: INode, Lexer, Parser, Evaluator

while (true)
{
    try
    {
        // Считываем строку с выражением
        Console.Write("> ");
        string input = Console.ReadLine();
        if (string.IsNullOrEmpty(input))
        {
            continue;
        }

        // Создаем класс для разбивки выражения на отдельные элементы (ноды)
        Lexer lexer = new Lexer(input);

        // Переводим инфиксную нотацию в польскую
        Queue<INode> expr = parse(lexer);

        // Вычисляем выражение
        double result = evaluate(expr);

        // Выводим результат
        Console.WriteLine(result.ToString());
    }
    // При исключениях выводим сообщения об ошибках
    catch (InvalidOperationException ex)
    {
        Console.WriteLine("invalid input");
    }
    catch (Exception ex)
    {
        Console.WriteLine(ex.Message);
    }
}

// Переводит строку из инфиксной нотации в префиксную
// Используется алгоритм Shunting-yard (https://en.wikipedia.org/wiki/Shunting-yard_algorithm)
Queue<INode> parse(Lexer lexer)
{
    Queue<INode> output_queue = new Queue<INode>();
    Stack<INode> operator_stack = new Stack<INode>();

    while (lexer.HasNext())
    {
        // Число помещаем в output queue
        INode node = lexer.NextNode();
        if (node.GetType() == typeof(Number))
        {
            output_queue.Enqueue(node);
            continue;
        }

        // Предположительно в этом месте остается только Operation
        if (node.GetType() != typeof(Operation))
        {
            throw new Exception("developer forgot to handle new types of Nodes");
        }

        Operation cur_op = (Operation) node;

        // Открывающую скобку помещаем в operator stack
        if (cur_op.Value == "(")
        {
            operator_stack.Push(cur_op);
            continue;
        }

        // Для закрывающей скобки ищем в operator stack открывающую скобку
        // Все элементы выше открывающей скобки в operator stack перемещаем в output queue
        if (cur_op.Value == ")")
        {
            do // Выполняем пока не нашли открывающую скобку
            {
                INode top_node;
                try
                {
                    top_node = operator_stack.Peek();
                }
                catch (InvalidOperationException ex)
                {
                    throw new InvalidOperationException("there is no opening paranthesis!");
                }

                // Проверим, не нашли ли мы открывающую скобку
                if (top_node.GetType() == typeof(Operation))
                {
                    Operation top_op = (Operation) top_node;
                    if (top_op.Value == "(")
                    {
                        operator_stack.Pop();
                        break;
                    }
                }

                // Если не нашли, перемещаем ноду из стэка в очередь
                output_queue.Enqueue(top_node);
                operator_stack.Pop();
            } while (true); // Пока не нашли открывающую скобку
            continue;
        }

        // Обрабатываем Operation, не являющийся скобками
        do
        {
            // Проверяем, есть ли на верху operator stack нода
            INode top_node;
            try
            {
                top_node = operator_stack.Peek();
            } catch (InvalidOperationException e)
            {
                break;
            }

            if (top_node.GetType() == typeof(Operation))
            {
                // Проверяем, не является ли нода со стека открывающей скобкой
                Operation top_op = (Operation)top_node;
                if (top_op.Value == "(")
                {
                    break;
                }

                // Сравниваем приоритеты нод (текущей и с верхушки стека)
                if (cur_op.Priority < top_op.Priority || (cur_op.Priority == top_op.Priority && cur_op.Associativity == "left"))
                {
                    // Если приоритет текущей операции меньше
                    // то перемещаем операцию с верхушки стека в очередь
                    operator_stack.Pop();
                    output_queue.Enqueue(top_node);
                    continue;
                }

                break;
            }
        } while (operator_stack.Count > 0);
        // Помещаем текущий оператор на стек
        operator_stack.Push(node);
    }

    // Оставшиеся в operator stack ноды перемещаем в output queue
    while (operator_stack.Count > 0)
    {
        // Если текущая нода это скобка, значит где-то есть незакрытые скобки
        INode top_node = operator_stack.Pop();
        if (top_node.GetType() == typeof(Operation))
        {
            Operation top_op = (Operation) top_node;
            if (top_op.Value == ")" || top_op.Value == "(")
            {
                throw new InvalidOperationException("mismatched parentheses!");
            }
        }
        output_queue.Enqueue(top_node);
    }

    return output_queue;
}

double evaluate(Queue<INode> expr)
{
    Stack<double> numbers = new Stack<double>();
    double lhs = 0;
    double rhs = 0;

    while (expr.Count > 0)
    {
        INode node = expr.Dequeue();
        if (node.GetType() == typeof(Number))
        {
            Number number = (Number) node;
            numbers.Push(number.Value);
            continue;
        }

        if (node.GetType() == typeof(Operation))
        {
            Operation op = (Operation) node;
            switch (op.Value)
            {
                case "+":
                    rhs = numbers.Pop();
                    lhs = numbers.Pop();
                    numbers.Push(lhs + rhs);
                    break;

                case "-":
                    rhs = numbers.Pop();
                    lhs = numbers.Pop();
                    numbers.Push(lhs - rhs);
                    break;

                case "*":
                    rhs = numbers.Pop();
                    lhs = numbers.Pop();
                    numbers.Push(lhs * rhs);
                    break;

                case "/":
                    rhs = numbers.Pop();
                    lhs = numbers.Pop();
                    if (rhs == 0)
                    {
                        throw new DivideByZeroException("can't divide by zero");
                    }
                    numbers.Push(lhs / rhs);
                    break;

                case "-u":
                    rhs = numbers.Pop();
                    numbers.Push(-rhs);
                    break;

                case "+u":
                    // Можем ничего не делать
                    // rhs = numbers.Pop();
                    //numbers.Push(+rhs);
                    break;
            }
        }
    }

    double result = numbers.Pop();
    return result;
}

// Используется как родительский класс для целых чисел, дробных чисел, функций, арифметических операций
interface INode
{
    public string ToString();
}

class Number: INode
{
    private readonly double value;

    public double Value
    {
        get { return value; }
    }

    override public string ToString()
    {
        return value.ToString();
    }

    public Number(double number)
    {
        value = number;
    }
}

class Operation: INode
{
    private readonly string value;
    private readonly uint priority;
    private readonly string associativity;

    public string Value
    {
        get { return value; }
    }

    public string Associativity
    {
        get { return associativity; }
    }

    public uint Priority
    {
        get { return priority; }
    }

    override public string ToString()
    {
        return value.ToString();
    }

    public Operation(string op, uint priority, string associativity = "left")
    {
        value = op;
        this.priority = priority;
        this.associativity = associativity;
    }
}

// Разбивает строку на ноды
class Lexer 
{
    private readonly string input;
    private int pos;
    private bool nextUnary;
    private bool prevIsNumber;

    public Lexer(string input)
    {
        this.input = input;
        this.pos = 0;
        this.nextUnary = true;
        this.prevIsNumber = false;
    }

    public bool HasNext()
    {
        if (pos < input.Length)
        {
            return true;
        }
        return false;
    }

    public INode NextNode()
    {
        while (pos < input.Length)
        {
            switch (input[pos])
            {
                case '+':
                    prevIsNumber = false;
                    pos++;

                    if (nextUnary)
                    {
                        return new Operation("+u", priority: 3, associativity: "right");
                    }

                    nextUnary = true;
                    return new Operation("+", priority: 1);

                case '-':
                    prevIsNumber = false;
                    pos++;

                    // Обрабатываем унарный минус
                    if (nextUnary)
                    {
                        return new Operation("-u", priority: 3, associativity: "right");
                    }

                    nextUnary = true;
                    return new Operation("-", priority: 1);

                case '*':
                    prevIsNumber = false;
                    nextUnary = true;
                    pos++;
                    return new Operation("*", priority: 2);

                case '/':
                    prevIsNumber = false;
                    nextUnary = true;
                    pos++;
                    return new Operation("/", priority: 2);

                case '(':
                    prevIsNumber = false;
                    nextUnary = true;
                    pos++;
                    return new Operation("(", priority: 4);

                case ')':
                    prevIsNumber = false;
                    nextUnary = false;
                    pos++;
                    return new Operation(")", priority: 4);

                case ' ':
                    pos++;
                    break;

                case ',':
                    // Считываем дробную часть
                    nextUnary = false;

                    // Если считали два числа подряд -- бросаем исключение
                    if (prevIsNumber)
                    {
                        throw new InvalidOperationException("missing operation!");
                    }
                    prevIsNumber = true;

                    int start_pos = pos;
                    pos += 1;
                    while (pos < input.Length && char.IsDigit(input[pos]))
                    {
                        pos++;
                    }

                    string str_number = string.Concat("0", input.AsSpan(start_pos, pos - start_pos));
                    double number = Convert.ToDouble(str_number);
                    return new Number(number);

                default:
                    nextUnary = false;

                    // Если не число -- возвращаем ошибку
                    if (!char.IsNumber(input[pos]))
                        throw new ArgumentException($"unknown symbol: {input[pos]}");

                    // Если считали два числа подряд -- бросаем исключение
                    if (prevIsNumber)
                    {
                        throw new InvalidOperationException("missing operation!");
                    }
                    prevIsNumber = true;

                    // Считываем число
                    start_pos = pos;
                    pos += 1;
                    while (pos < input.Length && char.IsDigit(input[pos]))
                    {
                        pos++;
                    }

                    if (pos < input.Length && input[pos] == ',')
                    {
                        pos++;
                        while (pos < input.Length && char.IsDigit(input[pos]))
                        {
                            pos++;
                        }
                    }

                    str_number = input.Substring(start_pos, pos-start_pos);
                    number = Convert.ToDouble(str_number);
                    return new Number(number);
            }
        }
        throw new ArgumentException("input error");
    }
}