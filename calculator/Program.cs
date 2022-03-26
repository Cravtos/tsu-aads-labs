// Используется алгоритм Shunting-yard (https://en.wikipedia.org/wiki/Shunting-yard_algorithm)

// Общая идея:
// 1. Считывается очередная строка, содержащая арифметическое выражение в инфиксной нотации
// 2. Создается класс Lexer, который разбивает строку на отдельные элементы -- токены (операции, числа, функции, и т.д.)
// 3. Последовательность токенов в выражении преобразуем из инфиксной нотации в постфиксную
//    по алгоритму Shunting-yard
// 4. Вычисляем и выводим результат выражения в постфиксной нотации

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

        // Создаем класс для выделения токенов в выражении
        Lexer lexer = new Lexer(input);

        // Переводим инфиксную нотацию в польскую
        Queue<IToken> expr = parse(lexer);

        // Вычисляем выражение
        double result = evaluate(expr);

        // Выводим результат
        Console.WriteLine(result.ToString());
    }
    // При исключениях выводим сообщения об ошибках
    catch (InvalidOperationException ex)
    {
        if (ex.Message.Contains("Stack empty"))
        {
            Console.WriteLine($"invalid input: missing number!");
            continue;
        }
        Console.WriteLine($"invalid input: {ex.Message}");
    }
    catch (Exception ex)
    {
        Console.WriteLine(ex.Message);
    }
}

// Переводит строку из инфиксной нотации в постфиксную
// Используется алгоритм Shunting-yard (https://en.wikipedia.org/wiki/Shunting-yard_algorithm)
Queue<IToken> parse(Lexer lexer)
{
    Queue<IToken> output_queue = new Queue<IToken>();
    Stack<IToken> operator_stack = new Stack<IToken>();

    while (lexer.HasNext())
    {
        // Число помещаем в output queue
        IToken token = lexer.NextToken();
        if (token.GetType() == typeof(Number))
        {
            output_queue.Enqueue(token);
            continue;
        }

        // Предполагаем, что в этом месте остается только Operation
        if (token.GetType() != typeof(Operation))
        {
            throw new Exception("developer forgot to handle new types of tokens");
        }

        Operation cur_op = (Operation) token;

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
                IToken top_token;
                try
                {
                    top_token = operator_stack.Peek();
                }
                catch (InvalidOperationException)
                {
                    throw new InvalidOperationException("there is no opening paranthesis!");
                }

                // Проверим, не нашли ли мы открывающую скобку
                if (top_token.GetType() == typeof(Operation))
                {
                    // Если нашли, убираем парную скобку со стека
                    Operation top_op = (Operation) top_token;
                    if (top_op.Value == "(")
                    {
                        operator_stack.Pop();
                        break;
                    }
                }

                // Если не нашли, перемещаем токен из стэка в очередь
                output_queue.Enqueue(top_token);
                operator_stack.Pop();
            } while (true);
            continue;
        }

        // Обрабатываем токен операции, не являющийся скобками
        do
        {
            // Проверяем, есть ли на верху operator stack токен
            IToken top_token;
            try
            {
                top_token = operator_stack.Peek();
            } catch (InvalidOperationException e)
            {
                break;
            }

            // Здесь обрабатывается приоритет операций
            if (top_token.GetType() == typeof(Operation))
            {
                // Проверяем, не является ли токен со стека открывающей скобкой
                Operation top_op = (Operation)top_token;
                if (top_op.Value == "(")
                {
                    break;
                }

                // Сравниваем приоритеты токенов (текущего и с верхушки стека) или при их равности -- ассоциативность операции.
                if (cur_op.Priority < top_op.Priority || (cur_op.Priority == top_op.Priority && cur_op.Associativity == "left"))
                {
                    // Если приоритет текущей операции меньше
                    // то перемещаем операцию с верхушки стека в очередь
                    operator_stack.Pop();
                    output_queue.Enqueue(top_token);
                    continue;
                }

                break;
            }
        } while (operator_stack.Count > 0);
        // Помещаем текущий оператор на стек
        operator_stack.Push(token);
    }

    // Оставшиеся в operator stack токены перемещаем в output queue
    while (operator_stack.Count > 0)
    {
        IToken top_token = operator_stack.Pop();

        // Если текущий токен это скобка, значит где-то есть незакрытые скобки
        if (top_token.GetType() == typeof(Operation))
        {
            Operation top_op = (Operation) top_token;
            if (top_op.Value == ")" || top_op.Value == "(")
            {
                throw new InvalidOperationException("mismatched parentheses!");
            }
        }

        output_queue.Enqueue(top_token);
    }

    return output_queue;
}

// Вычисляет выражение, данное в постфиксной записи
double evaluate(Queue<IToken> expr)
{
    Stack<double> numbers = new Stack<double>();
    double lhs = 0;
    double rhs = 0;

    // Пока есть токены в выражении:
    while (expr.Count > 0)
    {
        IToken token = expr.Dequeue();

        // Числа помещаем на стек
        if (token.GetType() == typeof(Number))
        {
            Number number = (Number) token;
            numbers.Push(number.Value);
            continue;
        }

        // Операции обрабатыаем, используя числа со стека
        if (token.GetType() == typeof(Operation))
        {
            Operation op = (Operation) token;
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

// Используется как родительский класс для чисел и операций
interface IToken
{
    public string ToString();
}

class Number: IToken
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

class Operation: IToken
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

// Разбивает выражение на токены
class Lexer 
{
    // Строка с арифметическим выражением
    private readonly string input;

    // На какой позиции в строке находится лексер
    private int pos;

    // Флаг того, что следующая операция будет унарной
    private bool nextUnary;

    // Флаг того, что прошлый токен был числом
    // Используется для обнаружений ошибок в выражении
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

    public IToken NextToken()
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

                // Считываем дробную часть
                case ',':
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