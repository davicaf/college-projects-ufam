import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        // 1. Entrada de dados para Fahrenheit
        System.out.print("Digite a temperatura em Fahrenheit: ");
        double tempFahrenheit = scanner.nextDouble();

        // 2. Encapsulamento na prática:
        // Criamos o objeto "conversor" passando o valor inicial pelo construtor.
        ConversorTemperatura conversor = new ConversorTemperatura(tempFahrenheit);

        // Chamamos o método que calcula a partir do valor encapsulado internamente
        double resultadoCelsius = conversor.converterFparaC();
        System.out.printf("%.2f°F equivalem a %.2f°C%n%n", conversor.getTemperatura(), resultadoCelsius);

        // 3. Entrada de dados para Celsius
        System.out.print("Digite a temperatura em Celsius: ");
        double tempCelsius = scanner.nextDouble();

        // 4. Encapsulamento na prática:
        // Usamos o 'setter' para atualizar com segurança a temperatura do objeto existente.
        conversor.setTemperatura(tempCelsius);

        // Chamamos o método para converter a nova temperatura
        double resultadoFahrenheit = conversor.converterCparaF();
        System.out.printf("%.2f°C equivalem a %.2f°F%n", conversor.getTemperatura(), resultadoFahrenheit);

        scanner.close();
    }
}
