import org.junit.Test;
import static org.junit.Assert.assertEquals;

public class ConversorTemperaturaTest {

    @Test
    public void testFahrenheitParaCelsius() {
        // Passando 32 Fahrenheit no construtor (32°F deve ser 0°C)
        ConversorTemperatura conversor = new ConversorTemperatura(32.0);
        
        // Verificando se o GET retorna o valor correto
        assertEquals(32.0, conversor.getTemperatura(), 0.01);
        
        // Verificando se a conversão dá 0 Celsius
        assertEquals(0.0, conversor.converterFparaC(), 0.01);
        assertEquals(0.0, conversor.converterAtualParaCelsius(), 0.01);
    }

    @Test
    public void testCelsiusParaFahrenheit() {
        // Usando um construtor com 0 Celsius
        ConversorTemperatura conversor = new ConversorTemperatura(0.0);
        
        // Testando o SET
        conversor.setTemperatura(0.0);
        
        // Verificando se a conversão de 0°C dá 32 Fahrenheit
        assertEquals(32.0, conversor.converterCparaF(), 0.01);
        assertEquals(32.0, conversor.converterAtualParaFahrenheit(), 0.01);
    }
}
