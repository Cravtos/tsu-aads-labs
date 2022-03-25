// See https://aka.ms/new-console-template for more information

uint towers_amount = 3;
uint rings_amount = 29;  // Общее количество колец на всех башнях

// Создаем башни. Изначально на нулевой башне находятся все кольца.
// (Сокращенная нотация для создания объекта)
Towers towers = new(towers_amount: towers_amount, rings_amount: rings_amount);

Console.WriteLine("Before: ");
towers.Print();

// Перемещаем кольца с первой башни на вторую, с использованием второй как промежуточной
try
{
    towers.MoveRings(rings_amount, from: towers.towers[0], tmp: towers.towers[1], to: towers.towers[2]);
} catch (Exception e)
{
    Console.WriteLine("Got exception {0}", e.Message);
}

// Выводим результат
Console.WriteLine("After: ");
towers.Print();

class Towers
{
    public readonly Tower[] towers;
    private readonly uint towers_amount = 0;
    public Towers(uint towers_amount, uint rings_amount)
    {
        this.towers_amount = towers_amount;
        towers = new Tower[towers_amount];
        towers[0] = new Tower(rings_amount: rings_amount);
        for (uint i = 1; i < towers_amount; i++)
        {
            towers[i] = new Tower(rings_amount: 0);
        }
    }

    public void Print()
    {
        for (uint i = 0; i < towers_amount; i++)
        {
            Console.Write("Tower {0}: ", i + 1);
            towers[i].Print();
        }
    }

    // Перемещает все кольца с одной башни на другую, с использованием промежуточной
    public void MoveRings(uint rings_amount, Tower from, Tower tmp, Tower to)
    {
        if (rings_amount == 1) // Перенос башни ширины 1
        {
            MoveRing(from, to);
            return;
        }

        // перекладываем (все кольца - 1 кольцо) на промежуточную башню
        MoveRings(rings_amount - 1, from: from, tmp: to, to: tmp);

        // Последнее кольцо перекладываем на нужную ось
        MoveRing(from, to);

        // Рекурсивно перекладываем остальные кольца на нужную башню
        MoveRings(rings_amount - 1, from: tmp, tmp: from, to: to);
    }

    // Перемещает кольцо с одной башни на другую
    static public void MoveRing(Tower from, Tower to)
    {
        uint ring; // Кольцо, которое нужно переложить
        try
        {
            ring = from.Pop();
        }
        catch (InvalidOperationException e)
        {
            // Если башня, из которой пытаются достать кольцо -- пустая,
            // то ничего не перекладываем.
            throw e; // Передаем исключение выше
        }

        try
        {
            to.Push(ring);
        }
        catch (InvalidOperationException e)
        {
            // Если попытались положить большее кольцо на меньшее,
            // возвращаем взятое кольцо на исходную башню
            from.Push(ring);
            throw e; // Передаем исключение выше
        }
    }
}

class Tower
{
    // Башня содержит кольца. Для их хранения выбрал стек, так как
    // нам нужно только брать элемент сверху и класт элемент сверху.
    // Стек делает обе эти операции за O(1).
    // Размер кольца определяется числом:
    // 1 -- кольцо наименьшего размера
    // 2 -- кольцо побольше
    private readonly Stack<uint> rings;

    // Создает и заполняет башню кольцами
    public Tower(uint rings_amount)
    {
        rings = new Stack<uint>(capacity: (int) rings_amount);
        for (uint i = rings_amount; i > 0; i--)
        {
            rings.Push(i);
        }
    }

    // Выводит кольца башни
    public void Print()
    {
        uint[] ringsAsArr = rings.ToArray();
        for (int i = ringsAsArr.Length - 1; i >= 0; i--)
        {
            Console.Write("{0} ", ringsAsArr[i]);
        }
        Console.WriteLine();
    }

    // Убирает кольцо с башни и возвращает его
    public uint Pop()
    {
        return rings.Pop();
    }

    // Помещает кольцо на башню
    public void Push(uint ring)
    {
        if (rings.Count == 0 || rings.Peek() >= ring)
        {
            rings.Push(ring);
        } else
        {
            throw new InvalidOperationException("Can't put bigger ring on a smaller one!");
        }
    }
}